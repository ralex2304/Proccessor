#include "spu.h"

LogFileData log_file = {STACK_LOG_FILENAME};

Status::Statuses SpuData::ctor() {
    int stk_res = STK_CTOR(&stk);

    if (stk_res != Stack::OK) {
        fprintf(stderr, CONSOLE_RED("Stack initialising error\n"));
        stk_print_error_to_user(stk_res);
        return Status::RUNTIME_ERROR;
    }

#ifdef GRAPHICS
    if (!sfml.ctor(PIXEL_SIZE, PIXEL_SIZE, VIDEO_WIDTH, VIDEO_HEIGHT, header)) {
        fprintf(stderr, CONSOLE_RED("sfml memory allocation error\n"));
        return Status::MEMORY_EXCEED;
    }
#endif //< #ifdef GRAPHICS

    return Status::NORMAL_WORK;
}

Status::Statuses SpuData::dtor() {
    int stk_res = stk_dtor(&stk);

    if (stk_res != Stack::OK) {
        fprintf(stderr, CONSOLE_RED("Stack error\n"));
        stk_print_error_to_user(stk_res);
        return Status::RUNTIME_ERROR;
    }

#ifdef GRAPHICS
    sfml.dtor();
#endif //< #ifdef GRAPHICS

    return Status::NORMAL_WORK;
}

void SpuData::dump() {
    printf("# spu dump():\n"
           "# \n");

    stk_data_dump(&stk);

    printf("# \n"
           "# Registers dump:\n");

    for (size_t i = 0; i < REGS_NUM; i++)
        printf("# %.*s = " IMM_DOUBLE_T_PRINTF "\n", String_PRINTF(REGS_DICT[i].name),
                                                     reg[REGS_DICT[i].num]);

    printf("# \n"
           "# dump end\n\n");

    fflush(stdout);
}

#define THROW_RUNTIME_ERROR_(text)  do {                                  \
            fprintf(stderr, CONSOLE_RED("Runtime error occured. " text)); \
            return Status::RUNTIME_ERROR;                                 \
        } while(0)

Status::Statuses spu_parse_and_execute(const char* data) {
    assert(data);

    size_t cur_byte = sizeof(FILE_HEADER);

    if (!((const FileHeader*) data)->check()) {
        THROW_RUNTIME_ERROR_("Invalid or corrupted file.");
        return Status::SIGNATURE_ERROR;
    }

    SpuData spu = {};
    STATUS_CHECK(spu.ctor());

    STATUS_CHECK(spu_execute(&spu, data, &cur_byte), spu.dtor());

    STATUS_CHECK(spu.dtor());

    return Status::NORMAL_WORK;
}

#undef THROW_RUNTIME_ERROR_

#define DATA_GET_VAL_(dest, val_t)  do {                                                \
                                        memcpy(&dest, data + *cur_byte, sizeof(val_t)); \
                                        *cur_byte += sizeof(val_t);                     \
                                    } while(0)

void spu_read_cmd(Cmd* cmd, const char* data, size_t *cur_byte) {
    assert(data);
    assert(cur_byte);

    DATA_GET_VAL_(cmd->keys, CmdKeys);

    if (cmd->keys.reg)
        DATA_GET_VAL_(cmd->args.reg, RegNum_t);

    if (cmd->keys.imm_double)
        DATA_GET_VAL_(cmd->args.imm_double, Imm_double_t);

    if (cmd->keys.imm_int)
        DATA_GET_VAL_(cmd->args.imm_int, Imm_int_t);
}

#undef DATA_GET_VAL_



#include "spu_dsl.h"

#define THROW_RUNTIME_ERROR_(...)  do {                                             \
            fprintf(stderr, CONSOLE_RED("Runtime error occured. ") __VA_ARGS__);    \
            fprintf(stderr, " Byte %zu\n", *cur_byte - cmd.size());                 \
            return Status::RUNTIME_ERROR;                                           \
        } while(0)

Status::Statuses spu_execute(SpuData* spu, const char* data, size_t* cur_byte) {
    assert(spu);
    assert(cur_byte);

    int stk_res = Stack::OK;

    Cmd cmd = {};

    static void* dispatch_table[] = {
                                     #include "cmd_dispatch_table/table.h"
                                                                          };

    #define DISPATCH()                      \
        spu_read_cmd(&cmd, data, cur_byte); \
        goto *dispatch_table[cmd.keys.num];

    DISPATCH();

    // Dispatch table labels:
    #define DEF_CMD(name, num, args, descr, ...)    \
        cmd_dispatch_do_##name:                     \
            __VA_ARGS__                             \
            DISPATCH();

    #include "cmd_dict.h"

    #undef DEF_CMD
    #undef DISPATCH
    // Dispatch table end


    cmd_dispatch_do_stk_error:
    fprintf(stderr, CONSOLE_RED("Runtime error occured!") " Stack error. Byte %zu\n",
                                                                        *cur_byte - cmd.size());
    stk_print_error_to_user(stk_res);
    return Status::RUNTIME_ERROR;

    cmd_dispatch_do_error:
    THROW_RUNTIME_ERROR_("Invalid command number %d.", cmd.keys.num);
}
#undef THROW_RUNTIME_ERROR_


#ifdef GRAPHICS

bool spu_sfml_show(SpuData* spu) {
    for (size_t i = 0; i < VIDEO_HEIGHT * VIDEO_WIDTH; i++)
        spu->sfml.set_pixel_color(i, (uint32_t)spu->ram[i]);

    spu->sfml.renew();

    return !spu->sfml.is_closed();
}

#endif //< #ifdef GRAPHICS

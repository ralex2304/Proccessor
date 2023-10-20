#include "spu.h"

LogFileData log_file = {STACK_LOG_FILENAME};

Status::Statuses SpuData::ctor() {
    int stk_res = STK_CTOR(&stk);

    if (stk_res != Stack::OK) {
        fprintf(stderr, CONSOLE_RED("Stack initialising error\n"));
        stk_print_error_to_user(stk_res);
        return Status::RUNTIME_ERROR;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses SpuData::dtor() {
    int stk_res = stk_dtor(&stk);

    if (stk_res != Stack::OK) {
        fprintf(stderr, CONSOLE_RED("Stack error\n"));
        stk_print_error_to_user(stk_res);
        return Status::RUNTIME_ERROR;
    }

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

#define THROW_RUNTIME_ERROR_(text)  do {                                                    \
            fprintf(stderr, CONSOLE_RED("Runtime error occured. " text) " Byte %zu\n", ip); \
            return Status::RUNTIME_ERROR;                                                   \
        } while(0)

#define DATA_GET_VAL_(dest, val_t)  do {                                                \
                                        memcpy(&dest, data + cur_byte, sizeof(val_t));  \
                                        cur_byte += sizeof(val_t);                      \
                                    } while(0)

Status::Statuses spu_parse(const char* data, const size_t size) {
    assert(data);

    size_t cur_byte = sizeof(FILE_HEADER);
    size_t ip = cur_byte;                   //< instruction pointer

    if (size < sizeof(FILE_HEADER) || !((const FileHeader*) data)->check()) {
        THROW_RUNTIME_ERROR_("Invalid or corrupted file.");
        return Status::SIGNATURE_ERROR;
    }

    SpuData spu = {};
    STATUS_CHECK(spu.ctor());

    Cmd cmd = {};

    while (cur_byte + sizeof(cmd.byte) <= size) {
        DATA_GET_VAL_(cmd.byte, CmdByte);

        cmd.info = find_command_by_num(cmd.byte.num);

        if (cmd.info == nullptr)
            THROW_RUNTIME_ERROR_("Invalid command.");

        if (cur_byte + cmd.size() > size) {
            stk_dtor(&spu.stk);

            THROW_RUNTIME_ERROR_("Arguments not found.");
        }

        if (cmd.byte.reg)
            DATA_GET_VAL_(cmd.args.reg, RegNum_t);

        if (cmd.byte.imm) {
            if (cmd.byte.ram || cmd.info->args.label)
                DATA_GET_VAL_(cmd.args.imm_int, Imm_int_t);
            else
                DATA_GET_VAL_(cmd.args.imm_double, Imm_double_t);
        }

        Status::Statuses res = spu_execute_command(&spu, &cmd, &cur_byte, ip);
        if (res != Status::NORMAL_WORK) {
            stk_dtor(&spu.stk);
            return res;
        }

        cmd = {};
        ip = cur_byte;
    }

    spu.dtor();

    THROW_RUNTIME_ERROR_("Program has no halt!");

    return Status::NORMAL_WORK;
}
#undef DATA_GET_VAL_
#undef THROW_RUNTIME_ERROR_



#include "spu_dsl.h"

#define THROW_RUNTIME_ERROR_(text)  do {                                                    \
            fprintf(stderr, CONSOLE_RED("Runtime error occured. " text) " Byte %zu\n", ip); \
            return Status::RUNTIME_ERROR;                                                   \
        } while(0)

Status::Statuses spu_execute_command(SpuData* spu, const Cmd* cmd, size_t* cur_byte,
                                     const size_t ip) {
    assert(spu);
    assert(cmd);
    assert(cur_byte);

    if (cmd->byte.ram)
        THROW_RUNTIME_ERROR_("SPU doesn't support RAM yet :-("); // FIXME

    int stk_res = Stack::OK;

    switch (cmd->info->num) {
        #define DEF_CMD(name, num, args, descr, ...)    \
            case CMD_##name:                            \
                __VA_ARGS__                             \
                break;

        #include "cmd_dict.h"

        #undef DEF_CMD

        default:
            assert(0 && "Wrong command num given");
            break;
    }

    if (stk_res != Stack::OK) {
        fprintf(stderr, CONSOLE_RED("Runtime error occured!") " Stack error. Byte %zu\n", ip);
        stk_print_error_to_user(stk_res);
        return Status::RUNTIME_ERROR;
    }

    return Status::NORMAL_WORK;
}
#undef THROW_RUNTIME_ERROR_


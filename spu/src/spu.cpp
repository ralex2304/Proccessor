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

    if (size < sizeof(SIGNATURE) || !((const Signature*) data)->check())
        return Status::SIGNATURE_ERROR;

    SpuData spu = {};
    STATUS_CHECK(spu.ctor());

    size_t cur_byte = sizeof(SIGNATURE);
    size_t ip = cur_byte;                   //< instruction pointer

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
            if (cmd.byte.ram)
                DATA_GET_VAL_(cmd.args.imm_ram, Imm_ram_t);
            else
                DATA_GET_VAL_(cmd.args.imm, Imm_t);
        }

        Status::Statuses res = spu_execute_command(&spu, &cmd, ip);
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



#include <spu_dsl.h>

#define THROW_RUNTIME_ERROR_(text)  do {                                                    \
            fprintf(stderr, CONSOLE_RED("Runtime error occured. " text) " Byte %zu\n", ip); \
            return Status::RUNTIME_ERROR;                                                   \
        } while(0)

#define DEF_CMD(name, num, arg1, arg2, arg3, descr, ...)    \
    case CMD_##name:                                        \
        __VA_ARGS__                                         \
        break;                                              \

Status::Statuses spu_execute_command(SpuData* spu, const Cmd* cmd, const size_t ip) {
    assert(spu);
    assert(cmd);

    if (cmd->byte.ram)
        THROW_RUNTIME_ERROR_("SPU doesn't support RAM yet :-("); // FIXME

    int stk_res = Stack::OK;

    switch (cmd->info->num) {
        #include "cmd_dict.h"

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
#undef DEF_CMD
#undef THROW_RUNTIME_ERROR_

// REVIEW can't undef DSL

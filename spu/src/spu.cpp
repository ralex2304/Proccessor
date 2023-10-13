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


#define STK_POP_AND_PUSH_ONE_(expression)   stk_res |= stk_pop(&spu->stk, &a);  \
                                                                                \
                                            if (stk_res == Stack::OK)           \
                                                stk_res |= stk_push(&spu->stk, expression)

#define STK_POP_AND_PUSH_TWO_(expression)   stk_res |= stk_pop(&spu->stk, &b);  \
                                                                                \
                                            if (stk_res != Stack::OK) break;    \
                                                                                \
                                            stk_res |= stk_pop(&spu->stk, &a);  \
                                                                                \
                                            if (stk_res == Stack::OK)           \
                                                stk_res |= stk_push(&spu->stk, expression)

Status::Statuses spu_execute_command(SpuData* spu, const Cmd* cmd, const size_t ip) {
    assert(spu);
    assert(cmd);

    if (cmd->byte.ram)
        THROW_RUNTIME_ERROR_("SPU doesn't support RAM yet :-("); // FIXME

    int stk_res = Stack::OK;

    double a = NAN;
    double b = NAN;

    switch (cmd->info->num) {
        case CMD_HLT:
            return Status::OK_EXIT;
        case CMD_PUSH:
            if (!cmd->byte.reg && !cmd->byte.imm)
                THROW_RUNTIME_ERROR_("\"push\" requires at least one argument.");

            a = 0;

            if (cmd->byte.reg)
                a += spu->reg[cmd->args.reg];

            if (cmd->byte.imm)
                a += cmd->args.imm;

            stk_res |= stk_push(&spu->stk, a);
            break;
        case CMD_POP:
            if (!cmd->byte.reg)
                THROW_RUNTIME_ERROR_("\"pop\" requires reg.");

            stk_res |= stk_pop(&spu->stk, &spu->reg[cmd->args.reg]);
            break;
        case CMD_IN:
            a = spu_get_double_from_input();
            if (isnan(a))
                return Status::WRONG_USER_INPUT;
            stk_res |= stk_push(&spu->stk, a);
            break;
        case CMD_OUT:
            stk_res |= stk_pop(&spu->stk, &a);

            if (stk_res == Stack::OK)
                if (!spu_print_double(a))
                    return Status::OUTPUT_ERROR;
            break;
        case CMD_ADD:
            STK_POP_AND_PUSH_TWO_(a + b);
            break;
        case CMD_SUB:
            STK_POP_AND_PUSH_TWO_(a - b);
            break;
        case CMD_MUL:
            STK_POP_AND_PUSH_TWO_(a * b);
            break;
        case CMD_DIV:
            STK_POP_AND_PUSH_TWO_(a / b);
            break;
        case CMD_SQRT:
            STK_POP_AND_PUSH_ONE_(sqrt(a));
            break;
        case CMD_SIN:
            STK_POP_AND_PUSH_ONE_(sin(a));
            break;
        case CMD_COS:
            STK_POP_AND_PUSH_ONE_(cos(a));
            break;
        default:
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
#undef STK_POP_AND_PUSH_ONE_
#undef STK_POP_AND_PUSH_TWO_

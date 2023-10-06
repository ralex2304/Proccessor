#include "spu.h"

LogFileData log_file = {STACK_LOG_FILENAME};


#define THROW_RUNTIME_ERROR_(text)  do {                                                    \
            fprintf(stderr, CONSOLE_RED("Runtime error occured. " text) " Byte %zu\n", op); \
            return Status::RUNTIME_ERROR;                                                   \
        } while(0)

#define DATA_GET_VAL_(dest, val_t)  do {                                            \
                                        dest = *((const val_t*)(data + cur_byte));  \
                                        cur_byte += sizeof(val_t);                  \
                                    } while(0)

Status::Statuses spu_parse(const char* data, const size_t size) {
    assert(data);


    SpuData spu = {};
    STK_CTOR(&spu.stk);

    size_t cur_byte = 0;
    size_t op = 0;

    CmdArgs cmd_args = {};
    CmdByte cmd_byte = {};

    const Cmd* cmd = nullptr;

    while (cur_byte + sizeof(cmd_byte) <= size) {
        DATA_GET_VAL_(cmd_byte, CmdByte);

        cmd = find_command_by_num(cmd_byte.num);

        if (cur_byte + cmd_byte.reg * sizeof(cmd_args.reg)
                     + cmd_byte.imm * sizeof(cmd_args.imm) > size) {
            stk_dtor(&spu.stk);

            THROW_RUNTIME_ERROR_("EOF instead of args");
        }

        if (cmd_byte.reg)
            DATA_GET_VAL_(cmd_args.reg, RegNum_t);

        if (cmd_byte.imm)
            DATA_GET_VAL_(cmd_args.imm, Imm_t);

        Status::Statuses res = spu_execute_command(&spu, cmd, &cmd_byte, &cmd_args, op);
        if (res != Status::NORMAL_WORK) {
            stk_dtor(&spu.stk);
            return res;
        }

        cmd = nullptr;
        cmd_byte = {};
        cmd_args = {};
        op = cur_byte;
    }

    stk_dtor(&spu.stk);

    THROW_RUNTIME_ERROR_("Program has no halt!"); // REVIEW

    return Status::NORMAL_WORK;
}
#undef DATA_GET_VAL_


#define STK_POP_AND_PUSH_ONE_(expression)   stk_res |= stk_pop(&spu->stk, &a);  \
                                                                                \
                                            if (stk_res == Stack::OK)           \
                                                stk_push(&spu->stk, expression)

#define STK_POP_AND_PUSH_TWO_(expression)   stk_res |= stk_pop(&spu->stk, &b);  \
                                            stk_res |= stk_pop(&spu->stk, &a);  \
                                                                                \
                                            if (stk_res == Stack::OK)           \
                                                stk_push(&spu->stk, expression)

Status::Statuses spu_execute_command(SpuData* spu, const Cmd* cmd, const CmdByte* cmd_byte,
                                     const CmdArgs* cmd_args, const size_t op) {
    assert(spu);
    assert(cmd);
    assert(cmd_byte);
    assert(cmd_args);

    int stk_res = Stack::OK;

    double a = NAN;
    double b = NAN;

    switch (cmd->num) {
        case CMD_HLT:
            return Status::OK_EXIT;
        case CMD_PUSH:
            if (!cmd_byte->reg && !cmd_byte->imm)
                THROW_RUNTIME_ERROR_("\"push\" requires at least one argument");

            a = 0;

            if (cmd_byte->reg)
                a += spu->reg[cmd_args->reg];

            if (cmd_byte->imm)
                a += cmd_args->imm;

            stk_res |= stk_push(&spu->stk, a);
            break;
        case CMD_POP:
            if (!cmd_byte->reg)
                THROW_RUNTIME_ERROR_("\"pop\" requires reg");

            stk_res |= stk_pop(&spu->stk, &spu->reg[cmd_args->reg]);
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
        fprintf(stderr, CONSOLE_RED("Runtime error occured! ") "Byte %zu\n", op);
        stk_print_error_to_user(stk_res);
        return Status::RUNTIME_ERROR;
    }

    return Status::NORMAL_WORK;
}
#undef THROW_RUNTIME_ERROR_

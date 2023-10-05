#include "spu.h"

LogFileData log_file = {STACK_LOG_FILENAME};

Status::Statuses spu_parse(const char* data, const size_t size) {
    assert(data);

    Stack stk = {};
    STK_CTOR(&stk);

    size_t current_byte = 0;
    size_t command_byte = 0;

    ArgVar args[COMMAND_MAX_ARGS_COUNT] = {};
    Command_num_t command_num = 0;

    const Command* command = nullptr;

    while (current_byte < size) {
        command_num = *((const Command_num_t*)(data + current_byte));
        current_byte += sizeof(Command_num_t);

        command = find_command_by_num(command_num);

        for (size_t i = 0; i < COMMAND_MAX_ARGS_COUNT && command->args[i] != ArgType::NONE; i++) {
            switch (command->args[i]) {
                case ArgType::DOUBLE:
                    args->double_arg = *((const double*)(data + current_byte));
                    break;
                case ArgType::NONE:
                default:
                    assert(0 && "Wrong ArgType passed");
            }

            current_byte += com_arg_type_sizeof(command->args[i]);
        }

        Status::Statuses res = spu_execute_command(&stk, command, args, command_byte);
        if (res != Status::NORMAL_WORK) {
            stk_dtor(&stk);
            return res;
        }

        command_num = 0;
        command = nullptr;
        command_byte = current_byte;
    }

    stk_dtor(&stk);

    return Status::NORMAL_WORK;
}


#define STK_POP_AND_PUSH_ONE_(expression)   stk_res |= stk_pop(stk, &a);    \
                                                                            \
                                            if (stk_res == Stack::OK)       \
                                                stk_push(stk, expression)   \

#define STK_POP_AND_PUSH_TWO_(expression)   stk_res |= stk_pop(stk, &b);    \
                                            stk_res |= stk_pop(stk, &a);    \
                                                                            \
                                            if (stk_res == Stack::OK)       \
                                                stk_push(stk, expression)   \

Status::Statuses spu_execute_command(Stack* stk, const Command* command, const ArgVar* args,
                                     const size_t command_byte) {
    assert(stk);
    assert(command);
    assert(args);

    int stk_res = Stack::OK;

    double a = NAN;
    double b = NAN;

    switch (command->num) {
    case COM_HLT:
        return Status::OK_EXIT;
    case COM_PUSH:
        stk_res |= stk_push(stk, args[0].double_arg);
        break;
    case COM_IN:
        a = spu_get_double_from_input();
        if (isnan(a))
            return Status::WRONG_USER_INPUT;
        stk_res |= stk_push(stk, a);
        break;
    case COM_OUT:
        stk_res |= stk_pop(stk, &a);

        if (stk_res == Stack::OK)
            if (!spu_print_double(a))
                return Status::OUTPUT_ERROR;
        break;
    case COM_ADD:
        STK_POP_AND_PUSH_TWO_(a + b);
        break;
    case COM_SUB:
        STK_POP_AND_PUSH_TWO_(a - b);
        break;
    case COM_MUL:
        STK_POP_AND_PUSH_TWO_(a * b);
        break;
    case COM_DIV:
        STK_POP_AND_PUSH_TWO_(a / b);
        break;
    case COM_SQRT:
        STK_POP_AND_PUSH_ONE_(sqrt(a));
        break;
    case COM_SIN:
        STK_POP_AND_PUSH_ONE_(sin(a));
        break;
    case COM_COS:
        STK_POP_AND_PUSH_ONE_(cos(a));
        break;
    default:
        break;
    }

    if (stk_res != Stack::OK) {
        printf(CONSOLE_RED("Runtime error occured! ") "Byte %zu\n", command_byte);
        stk_print_error_to_user(stk_res);
        return Status::RUNTIME_ERROR;
    }

    return Status::NORMAL_WORK;
}

const Command* find_command_by_num(const Command_num_t num) {
    if (num == COMMANDS[num].num)
        return COMMANDS + num;

    assert(0 && "Command num and index in COMMANDS array must be the same");
}

#include "disassembler.h"

#define THROW_SYNTAX_ERROR_(text)  do {                                             \
            fprintf(stderr, CONSOLE_RED("Syntax error. " text) " Byte %zu\n", op);  \
            return Status::SYNTAX_ERROR;                                           \
        } while(0)

#define DATA_GET_VAL_(dest, val_t)  do {                                            \
                                        dest = *((const val_t*)(data + cur_byte));  \
                                        cur_byte += sizeof(val_t);                  \
                                    } while(0)

#define F_PRINTF_(...)  do {                                        \
                            if (!file_printf(file, __VA_ARGS__))    \
                                return Status::OUT_FILE_ERROR;      \
                        } while(0)

Status::Statuses disasm_parse(const char* data, const size_t size, const char* output_filename,
                              const bool debug_mode) {
    assert(data);
    assert(output_filename);

    FILE* file = {};
    if (!file_open(&file, output_filename, "wb"))
        return Status::OUT_FILE_ERROR;

    size_t cur_byte = 0;
    size_t op = 0;

    const Cmd* cmd = nullptr;
    CmdByte cmd_byte = {};
    CmdArgs cmd_args = {};


    while (cur_byte + sizeof(cmd_byte) <= size) {
        DATA_GET_VAL_(cmd_byte, CmdByte);

        cmd = find_command_by_num(cmd_byte.num);

        if (cmd == nullptr)
            THROW_SYNTAX_ERROR_("Unknown command number");

        F_PRINTF_("%s", cmd->name);

        if (cmd_byte.reg) {
            DATA_GET_VAL_(cmd_args.reg, RegNum_t);

            F_PRINTF_(" %s", find_reg_by_num(cmd_args.reg)->name);
        }

        if (cmd_byte.imm) {
            if (cmd_byte.reg)
                F_PRINTF_("+");
            else
                F_PRINTF_(" ");

            DATA_GET_VAL_(cmd_args.imm, Imm_t);

            F_PRINTF_(IMM_T_PRINTF, cmd_args.imm);
        }

        if (debug_mode)
            F_PRINTF_("%*s;byte=%zu", 10, "", op);

        F_PRINTF_("\n");

        op = cur_byte;
    }

    if (!file_close(file))
        return Status::OUT_FILE_ERROR;

    return Status::NORMAL_WORK;
}
#undef F_PRINTF_
#undef DATA_GET_VAL_
#undef THROW_SYNTAX_ERROR_


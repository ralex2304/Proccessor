#include "disassembler.h"

#define THROW_SYNTAX_ERROR_(text)  do {                                             \
            fprintf(stderr, CONSOLE_RED("Syntax error. " text) " Byte %zu\n", ip);  \
            return Status::SYNTAX_ERROR;                                           \
        } while(0)

#define DATA_GET_VAL_(dest, val_t)  do {                                                \
                                        memcpy(&dest, data + cur_byte, sizeof(val_t));  \
                                        cur_byte += sizeof(val_t);                      \
                                    } while(0)

#define F_PRINTF_(...)  do {                                                    \
                            int printf_res = file_printf(file, __VA_ARGS__);    \
                            if (printf_res == EOF) {                            \
                                file_close(file);                               \
                                return Status::OUT_FILE_ERROR;                  \
                            }                                                   \
                            printed_cnt += printf_res;                          \
                        } while(0)

Status::Statuses disasm_parse(const char* data, const size_t size, const char* output_filename,
                              const bool debug_mode) {
    assert(data);
    assert(output_filename);

    FileHeader header = {};

    if (size < sizeof(FILE_HEADER) || !(header = *((const FileHeader*) data)).check())
        return Status::SIGNATURE_ERROR;

    FILE* file = {};
    if (!file_open(&file, output_filename, "wb"))
        return Status::OUT_FILE_ERROR;

    if (disasm_write_header(file, header) == EOF) {
        file_close(file);
        return Status::OUT_FILE_ERROR;
    }

    size_t cur_byte = sizeof(FILE_HEADER);
    size_t ip = cur_byte;                 //< Instruction pointer

    Cmd cmd = {};

    while (cur_byte + sizeof(cmd.keys) <= size) {
        DATA_GET_VAL_(cmd.keys, CmdKeys);

        cmd.info = find_command_by_num(cmd.keys.num);

        if (cmd.info == nullptr)
            THROW_SYNTAX_ERROR_("Unknown command number.");

        if (cur_byte + cmd.size() - sizeof(cmd.keys) > size)
            THROW_SYNTAX_ERROR_("Arguments not found.");

        int printed_cnt = 0;

        F_PRINTF_("%.*s", String_PRINTF(cmd.info->name));

        if (cmd.keys.ram)
            F_PRINTF_(" [");
        else if (cmd.keys.reg || cmd.keys.imm_int ||cmd.keys.imm_double)
            F_PRINTF_(" ");

        if (cmd.keys.reg) {
            DATA_GET_VAL_(cmd.args.reg, RegNum_t);

            F_PRINTF_("%.*s", String_PRINTF(find_reg_by_num(cmd.args.reg)->name));

            if (cmd.keys.imm_int || cmd.keys.imm_double)
                F_PRINTF_("+");
        }

        if (cmd.keys.imm_double) {
            DATA_GET_VAL_(cmd.args.imm_double, Imm_double_t);
            F_PRINTF_(IMM_DOUBLE_T_PRINTF, cmd.args.imm_double);

            if (cmd.keys.imm_int)
                F_PRINTF_("+");
        }

        if (cmd.keys.imm_int) {
            DATA_GET_VAL_(cmd.args.imm_int, Imm_int_t);
            F_PRINTF_(IMM_INT_T_PRINTF, cmd.args.imm_int);
        }

        if (cmd.keys.ram)
            F_PRINTF_("]");

        if (debug_mode) {
            static const int DISASM_COMMENTS_OFFSET = 50;

            if (printed_cnt < DISASM_COMMENTS_OFFSET)
                F_PRINTF_("%*s", DISASM_COMMENTS_OFFSET - printed_cnt, "");

            F_PRINTF_(";addr = %04zu", ip);
        }

        F_PRINTF_("\n");

        ip = cur_byte;
    }

    if (!file_close(file))
        return Status::OUT_FILE_ERROR;

    return Status::NORMAL_WORK;
}
#undef F_PRINTF_
#undef DATA_GET_VAL_
#undef THROW_SYNTAX_ERROR_

#define F_PRINTF_CHECK_(printf) do {                    \
                                    if (printf == EOF)  \
                                        return EOF;     \
                                } while(0)

int disasm_write_header(FILE* file, const FileHeader header) {
    assert(file);

    static_assert(sizeof(header.sign) == 2);

    F_PRINTF_CHECK_(file_printf(file, ";Signature: %c%c\n"
                                      ";Version: %d\n",
                                      header.sign, header.sign >> 8, header.version));

    return 1;
}
#undef F_PRINTF_CHECK_


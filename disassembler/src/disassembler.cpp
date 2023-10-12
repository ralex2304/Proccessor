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

    Signature sign = {};

    if (size < sizeof(SIGNATURE) || !(sign = *((const Signature*) data)).check())
        return Status::SIGNATURE_ERROR;

    FILE* file = {};
    if (!file_open(&file, output_filename, "wb"))
        return Status::OUT_FILE_ERROR;

    if (disasm_write_signature(file, sign) == EOF) {
        file_close(file);
        return Status::OUT_FILE_ERROR;
    }

    size_t cur_byte = sizeof(SIGNATURE);
    size_t ip = cur_byte;                 //< Instruction pointer

    Cmd cmd = {};

    while (cur_byte + sizeof(cmd.byte) <= size) {
        DATA_GET_VAL_(cmd.byte, CmdByte);

        cmd.info = find_command_by_num(cmd.byte.num);

        if (cmd.info == nullptr)
            THROW_SYNTAX_ERROR_("Unknown command number");

        size_t printed_cnt = 0;

        F_PRINTF_("%s", cmd.info->name);

        if (cmd.byte.ram)
            F_PRINTF_(" [");
        else if (cmd.byte.imm || cmd.byte.reg)
            F_PRINTF_(" ");

        if (cmd.byte.reg) {
            DATA_GET_VAL_(cmd.args.reg, RegNum_t);

            F_PRINTF_("%s", find_reg_by_num(cmd.args.reg)->name);
        }

        if (cmd.byte.imm) {
            if (cmd.byte.reg)
                F_PRINTF_("+");

            if (cmd.byte.ram) {
                DATA_GET_VAL_(cmd.args.imm_ram, Imm_ram_t);
                F_PRINTF_(IMM_RAM_T_PRINTF, cmd.args.imm_ram);
            } else {
                DATA_GET_VAL_(cmd.args.imm, Imm_t);
                F_PRINTF_(IMM_T_PRINTF, cmd.args.imm);
            }
        }

        if (cmd.byte.ram)
            F_PRINTF_("]");

        if (debug_mode) {
            static const size_t DISASM_COMMENTS_OFFSET = 50;

            if (printed_cnt < DISASM_COMMENTS_OFFSET)
                F_PRINTF_("%*s", DISASM_COMMENTS_OFFSET - printed_cnt, "");

            F_PRINTF_(";byte=%zu", ip);
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

int disasm_write_signature(FILE* file, const Signature sign) {
    assert(file);

    static_assert(sizeof(sign.name) == 2);

    F_PRINTF_CHECK_(file_printf(file, ";Signature: %c%c\n"
                                      ";Version: %d\n",
                                      sign.name[0], sign.name[1], sign.version));

    return 1;
}
#undef F_PRINTF_CHECK_


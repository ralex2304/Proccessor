#include "assembler.h"

Status::Statuses assmeble_and_write(const InputData* input_data, const char* filename,
                                    bool debug_mode) {
    assert(input_data);
    assert(filename);

    FILE* file = {};
    if (!file_open(&file, filename, "wb"))
        return Status::OUT_FILE_ERROR;

    Status::Statuses res = Status::DEFAULT;

    if (debug_mode)
        if (file_printf(file, ";line - byte - command\n") == EOF)
            return Status::OUT_FILE_ERROR;

    for (size_t i = 0; i < input_data->lines_cnt; i++) {
        res = parse_and_write_line(file, input_data->lines[i], i + 1, debug_mode);

        if (res != Status::NORMAL_WORK) {
            file_close(file);
            return res;
        }
    }

    if (!file_close(file))
        return Status::OUT_FILE_ERROR;

    return Status::NORMAL_WORK;
}

#define THROW_SYNTAX_ERROR_(text, ...)  do {                                                        \
            fprintf(stderr, CONSOLE_RED("Syntax error. " text) " Line %zu\n", __VA_ARGS__ line_num);\
            return Status::SYNTAX_ERROR;                                                            \
        } while(0)

Status::Statuses parse_and_write_line(FILE* file, String line, const size_t line_num,
                                      bool debug_mode) {
    assert(file);
    assert(line.str);

    Cmd cmd = {};

    static size_t binary_pos = 0;
    char inp_buf[MAX_LINE_LEN] = {};

    text_throw_out_comment(line.str);

    line.str = strtok(line.str, " ");
    int res = sscanf(line.str, "%s", inp_buf);

    if (res != 1)
        THROW_SYNTAX_ERROR_("Can't read command name.");

    cmd.info = find_command_by_name(inp_buf);
    if (cmd.info == nullptr)
        THROW_SYNTAX_ERROR_("Command not found.");

    cmd.byte.num = cmd.info->num & CMD_BYTE_NUM_BIT_MASK;

    if (cmd.info->args.reg) {
        line.str = strtok(nullptr, " +");
        STATUS_CHECK(asm_read_reg(line.str, &cmd, line_num));
    }

    if (cmd.info->args.imm) {
        if (!cmd.info->args.reg || cmd.byte.reg)
            line.str = strtok(nullptr, " ");

        STATUS_CHECK(asm_read_imm(line.str, &cmd, line_num));
    }

    if ((cmd.info->args.imm || cmd.info->args.reg) &&
        !(cmd.byte.imm || cmd.byte.reg))
            THROW_SYNTAX_ERROR_("At least one argument required.");

    return asm_write_cmd(file, &cmd, &binary_pos, line_num, debug_mode);
}

Status::Statuses asm_read_reg(const char* str, Cmd* cmd, const size_t line_num) {
    assert(str);
    assert(cmd);

    if (str != nullptr && isalpha(str[0])) {
        const RegInfo* reg = find_reg_by_name(str);

        if (reg == nullptr)
            THROW_SYNTAX_ERROR_("Invalid reg name \"%s\"", str,);

        cmd->args.reg = reg->num;
        cmd->byte.reg = true;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_imm(const char* str, Cmd* cmd, const size_t line_num) {
    assert(str);
    assert(cmd);

    if (str != nullptr) {
        int sscanf_res = sscanf(str, IMM_T_PRINTF, &cmd->args.imm);

        if (sscanf_res != 1)
            THROW_SYNTAX_ERROR_("Invalid immediate argument \"%s\"", str,);

        cmd->byte.imm = true;
    }

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_


Status::Statuses asm_write_cmd(FILE* file, const Cmd* cmd, size_t* binary_pos,
                               const size_t line_num, const bool debug_mode) {
    assert(file);
    assert(cmd);

    Status::Statuses res = Status::DEFAULT;

    if (debug_mode)
        res = asm_write_cmd_debug(file, cmd, *binary_pos, line_num);
    else
        res = asm_write_cmd_bin(file, cmd);

    if (res != Status::NORMAL_WORK)
        return res;

    *binary_pos += sizeof(cmd->byte) + cmd->byte.reg * sizeof(cmd->args.reg)
                                     + cmd->byte.imm * sizeof(cmd->args.imm);

    return res;
}


#define F_PRINTF_CHECK_(printf)    if (printf == EOF) return Status::OUT_FILE_ERROR

Status::Statuses asm_write_cmd_debug(FILE* file, const Cmd* cmd,
                                     const size_t binary_pos, const size_t line_num) {
    assert(file);
    assert(cmd);

    F_PRINTF_CHECK_(file_printf(file, ";%4zu - %4zu -", line_num, binary_pos));

    // Command in hex format

    F_PRINTF_CHECK_(file_printf(file, " %02X", cmd->byte));

    if (cmd->byte.reg)
        for (size_t i = 0; i < sizeof(cmd->args.reg); i++)
            F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->args.reg)[i]));

    if (cmd->byte.imm)
        for (size_t i = 0; i < sizeof(cmd->args.imm); i++)
            F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->args.imm)[i]));

    F_PRINTF_CHECK_(file_printf(file, "%*s", (!cmd->byte.reg) * sizeof(cmd->args.reg) * 3 +
                                             (!cmd->byte.imm) * sizeof(cmd->args.imm) * 3, ""));

    // Command in text format

    F_PRINTF_CHECK_(file_printf(file, " - %s", cmd->info->name));

    if (cmd->byte.reg)
        F_PRINTF_CHECK_(file_printf(file, " %s", find_reg_by_num(cmd->args.reg)->name));

    if (cmd->byte.imm) {
        if (cmd->byte.reg) {
            F_PRINTF_CHECK_(file_printf(file, "+"));
        } else {
            F_PRINTF_CHECK_(file_printf(file, " "));
        }

        F_PRINTF_CHECK_(file_printf(file, IMM_T_PRINTF, cmd->args.imm));
    }

    F_PRINTF_CHECK_(file_printf(file, "\n"));

    return Status::NORMAL_WORK;
}
#undef F_PRINTF_CHECK_


#define F_WRITE_CHECK_(write)    if (!write) return Status::OUT_FILE_ERROR

Status::Statuses asm_write_cmd_bin(FILE* file, const Cmd* cmd) {
    assert(file);
    assert(cmd);

    F_WRITE_CHECK_(file_write_bytes(file, &cmd->byte, sizeof(cmd->byte)));

    if (cmd->byte.reg)
        F_WRITE_CHECK_(file_write_bytes(file, &cmd->args.reg, sizeof(cmd->args.reg)));

    if (cmd->byte.imm)
        F_WRITE_CHECK_(file_write_bytes(file, &cmd->args.imm, sizeof(cmd->args.imm)));

    return Status::NORMAL_WORK;
}
#undef F_WRITE_CHECK_

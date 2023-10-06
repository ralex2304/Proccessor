#include "assembler.h"

Status::Statuses assmeble_and_write(const InputData* input_data, const char* filename,
                                    bool debug_mode) {
    assert(input_data);
    assert(filename);

    FILE* file = {};
    if (!file_open(&file, filename, "wb"))
        return Status::OUT_FILE_ERROR;

    Status::Statuses res = Status::DEFAULT;

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

#define THROW_SYNTAX_ERROR_(text, ...)  do {                                               \
            fprintf(stderr, CONSOLE_RED("Syntax error. " text) " Line %zu\n", __VA_ARGS__ line_num);\
            return Status::SYNTAX_ERROR;                                                            \
        } while(0)

Status::Statuses parse_and_write_line(FILE* file, String line, const size_t line_num,
                                      bool debug_mode) {
    assert(file);
    assert(line.str);

    CmdByte cmd_byte = {};
    CmdArgs cmd_args = {};
    static size_t binary_pos = 0;
    char inp_buf[MAX_LINE_LEN] = {};

    text_throw_out_comment(line.str);

    line.str = strtok(line.str, " ");
    int res = sscanf(line.str, "%s", inp_buf);

    if (res != 1)
        THROW_SYNTAX_ERROR_("Can't read command name.");

    const Cmd* cmd = find_command_by_name(inp_buf);
    if (cmd == nullptr)
        THROW_SYNTAX_ERROR_("Command not found.");

    cmd_byte.num = cmd->num & 0b00011111; // REVIEW

    if (cmd->args.reg) {
        line.str = strtok(nullptr, " +");
        STATUS_CHECK(asm_read_reg(line.str, &cmd_byte, &cmd_args, line_num));
    }

    if (cmd->args.imm) {
        if (!cmd->args.reg || cmd_byte.reg)
            line.str = strtok(nullptr, " ");

        STATUS_CHECK(asm_read_imm(line.str, &cmd_byte, &cmd_args, line_num));
    }

    if ((cmd->args.imm || cmd->args.reg) &&
        !(cmd_byte.imm || cmd_byte.reg))
            THROW_SYNTAX_ERROR_("At least one argument required.");

    return asm_write_cmd(file, &cmd_byte, &cmd_args, &binary_pos, debug_mode);
}

Status::Statuses asm_read_reg(const char* str, CmdByte* cmd_byte, CmdArgs* cmd_args,
                              const size_t line_num) {
    if (str != nullptr && isalpha(str[0])) {
        const Reg* reg = find_reg_by_name(str);

        if (reg == nullptr)
            THROW_SYNTAX_ERROR_("Invalid reg name \"%s\"", str,);

        cmd_args->reg = reg->num;
        cmd_byte->reg = true;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_imm(const char* str, CmdByte* cmd_byte, CmdArgs* cmd_args,
                              const size_t line_num) {
    if (str != nullptr) {
        int sscanf_res = sscanf(str, IMM_T_PRINTF, &cmd_args->imm);

        if (sscanf_res != 1)
            THROW_SYNTAX_ERROR_("Invalid immediate argument \"%s\"", str,);

        cmd_byte->imm = true;
    }

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_


Status::Statuses asm_write_cmd(FILE* file, const CmdByte* cmd_byte, const CmdArgs* cmd_args,
                               size_t* binary_pos, const bool debug_mode) {
    assert(file);
    assert(cmd_byte);
    assert(cmd_args);

    Status::Statuses res = Status::DEFAULT;

    if (debug_mode)
        res = asm_write_cmd_text(file, cmd_byte, cmd_args, *binary_pos);
    else
        res = asm_write_cmd_bin(file, cmd_byte, cmd_args);

    if (res != Status::NORMAL_WORK)
        return res;

    *binary_pos += sizeof(*cmd_byte) + cmd_byte->reg * sizeof(cmd_args->reg)
                                     + cmd_byte->imm * sizeof(cmd_args->imm);

    return res;
}


#define F_PRINTF_CHECK_(printf)    if (!printf) return Status::OUT_FILE_ERROR

Status::Statuses asm_write_cmd_text(FILE* file, const CmdByte* cmd_byte,
                                    const CmdArgs* cmd_args, const size_t binary_pos) {
    assert(file);
    assert(cmd_byte);
    assert(cmd_args);


    F_PRINTF_CHECK_(file_printf(file, ";(%5zu) %d", binary_pos, cmd_byte->num));

    if (cmd_byte->reg)
        F_PRINTF_CHECK_(file_printf(file, " %d", cmd_args->reg));

    if (cmd_byte->imm) {
        if (cmd_byte->reg) {
            F_PRINTF_CHECK_(file_printf(file, "+"));
        } else {
            F_PRINTF_CHECK_(file_printf(file, " "));
        }

        F_PRINTF_CHECK_(file_printf(file, IMM_T_PRINTF, cmd_args->imm));
    }

    F_PRINTF_CHECK_(file_printf(file, "\n"));

    return Status::NORMAL_WORK;
}
#undef F_PRINTF_CHECK_


#define F_WRITE_CHECK_(write)    if (!write) return Status::OUT_FILE_ERROR

Status::Statuses asm_write_cmd_bin(FILE* file, const CmdByte* cmd_byte,
                                   const CmdArgs* cmd_args) {
    assert(file);
    assert(cmd_byte);
    assert(cmd_args);

    F_WRITE_CHECK_(file_write_bytes(file, cmd_byte, sizeof(*cmd_byte)));

    if (cmd_byte->reg)
        F_WRITE_CHECK_(file_write_bytes(file, &cmd_args->reg, sizeof(cmd_args->reg)));

    if (cmd_byte->imm)
        F_WRITE_CHECK_(file_write_bytes(file, &cmd_args->imm, sizeof(cmd_args->imm)));

    return Status::NORMAL_WORK;
}
#undef F_WRITE_CHECK_

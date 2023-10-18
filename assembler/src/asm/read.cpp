#include "read.h"

#define THROW_SYNTAX_ERROR_(text, ...)  do {                                                        \
            fprintf(stderr, CONSOLE_RED("Syntax error. " text) " Line %zu\n", __VA_ARGS__ line_num);\
            return Status::SYNTAX_ERROR;                                                            \
        } while(0)

Status::Statuses asm_read_args_ram(const char** tokens, size_t* cur_token, Cmd* cmd, const size_t line_num) {
    assert(tokens);
    assert(cur_token);
    assert(cmd);

    if (!cmd->info->args.ram)
        THROW_SYNTAX_ERROR_("Command doesn't support ram args.");

    cmd->byte.ram = true;

    STATUS_CHECK(asm_read_arg_reg(tokens, cur_token, cmd, line_num));

    if (tokens[*cur_token] != nullptr && tokens[*cur_token][0] != ']')
        STATUS_CHECK(asm_read_arg_imm_int(tokens, cur_token, cmd, line_num));

    if (tokens[*cur_token] != nullptr && tokens[*cur_token][0] != ']')
        THROW_SYNTAX_ERROR_("Expected \"]\".");

    *cur_token += 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_args(const char** tokens, size_t* cur_token, Cmd* cmd,
                               JumpLabel* labels, const size_t line_num, const bool first_pass) {
    assert(tokens);
    assert(cur_token);
    assert(cmd);

    if (cmd->info->args.label) {
        STATUS_CHECK(asm_get_label(labels, tokens[*cur_token], &cmd->args.imm_int,
                                   line_num, first_pass));

        cmd->byte.imm = true;
        *cur_token += 1;

        return Status::NORMAL_WORK;
    }

    STATUS_CHECK(asm_read_arg_reg(tokens, cur_token, cmd, line_num));
    STATUS_CHECK(asm_read_arg_imm(tokens, cur_token, cmd, line_num));

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_reg(const char** tokens, size_t* cur_token,
                                  Cmd* cmd, const size_t line_num) {
    assert(cmd);
    assert(tokens);

    if (!cmd->info->args.reg || tokens[*cur_token] == nullptr)
        return Status::NORMAL_WORK;

    if (isalpha(tokens[*cur_token][0])) {
        const RegInfo* reg = find_reg_by_name(tokens[*cur_token]);

        if (reg == nullptr)
            THROW_SYNTAX_ERROR_("Invalid reg name \"%s\"", tokens[*cur_token],);

        cmd->args.reg = reg->num;
        cmd->byte.reg = true;

        *cur_token += 1;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm(const char** tokens, size_t* cur_token,
                                  Cmd* cmd, const size_t line_num) {
    assert(cmd);
    assert(tokens);

    if (tokens[*cur_token] == nullptr)
        return Status::NORMAL_WORK;

    asm_read_arg_imm_double(tokens, cur_token, cmd, line_num);
    asm_read_arg_imm_int   (tokens, cur_token, cmd, line_num);

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm_int(const char** tokens, size_t* cur_token,
                                      Cmd* cmd, const size_t line_num) {
    assert(cmd);
    assert(tokens);

    if (!cmd->info->args.imm_int || tokens[*cur_token] == nullptr)
        return Status::NORMAL_WORK;

    int sscanf_res = sscanf(tokens[*cur_token], IMM_INT_T_PRINTF, &cmd->args.imm_int);

    if (sscanf_res != 1)
        THROW_SYNTAX_ERROR_("Invalid immediate argument \"%s\"", tokens[*cur_token],);

    cmd->byte.imm = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm_double(const char** tokens, size_t* cur_token,
                                         Cmd* cmd, const size_t line_num) {
    assert(cmd);
    assert(tokens);

    if (!cmd->info->args.imm_double || tokens[*cur_token] == nullptr)
        return Status::NORMAL_WORK;

    int sscanf_res = sscanf(tokens[*cur_token], IMM_DOUBLE_T_PRINTF, &cmd->args.imm_double);

    if (sscanf_res != 1)
        THROW_SYNTAX_ERROR_("Invalid immediate argument \"%s\"", tokens[*cur_token],);

    cmd->byte.imm = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

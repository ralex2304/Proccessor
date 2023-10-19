#include "read.h"

#define THROW_SYNTAX_ERROR_(...)  return asm_throw_syntax_error(tokens[*cur_token], \
                                                                inp_file,           \
                                                                __VA_ARGS__)

Status::Statuses asm_read_args_ram(String* tokens, size_t* cur_token, Cmd* cmd,
                                   const InputFileInfo* inp_file) {
    assert(tokens);
    assert(cur_token);
    assert(cmd);
    assert(inp_file);

    if (!cmd->info->args.ram)
        THROW_SYNTAX_ERROR_("Command doesn't support ram args.");

    cmd->byte.ram = true;

    STATUS_CHECK(asm_read_arg_reg(tokens, cur_token, cmd, inp_file));

    if (tokens[*cur_token].str != nullptr && tokens[*cur_token].len != 0
        && tokens[*cur_token].str[0] != ']')
            STATUS_CHECK(asm_read_arg_imm_int(tokens, cur_token, cmd, inp_file));

    if (tokens[*cur_token].len == 0 || tokens[*cur_token].str[0] != ']')
        THROW_SYNTAX_ERROR_("Expected \"]\".");

    *cur_token += 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_args(String* tokens, size_t* cur_token, Cmd* cmd,
                               const InputFileInfo* inp_file,
                               JumpLabel* labels, const bool final_pass) {
    assert(tokens);
    assert(cur_token);
    assert(cmd);
    assert(inp_file);

    if (cmd->info->args.label) {
        STATUS_CHECK(asm_get_label_byte(labels, tokens[*cur_token], inp_file,
                                        &cmd->args.imm_int, final_pass));

        cmd->byte.imm = true;
        *cur_token += 1;

        return Status::NORMAL_WORK;
    }

    STATUS_CHECK(asm_read_arg_reg(tokens, cur_token, cmd, inp_file));
    STATUS_CHECK(asm_read_arg_imm(tokens, cur_token, cmd, inp_file));

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_reg(String* tokens, size_t* cur_token, Cmd* cmd,
                                  const InputFileInfo* inp_file) {
    assert(cmd);
    assert(tokens);
    assert(inp_file);

    if (!cmd->info->args.reg || tokens[*cur_token].len == 0)
        return Status::NORMAL_WORK;

    if (isalpha(tokens[*cur_token].str[0])) {
        const RegInfo* reg = find_reg_by_name(tokens[*cur_token]);

        if (reg == nullptr)
            THROW_SYNTAX_ERROR_("Invalid reg name \"%.*s\"", tokens[*cur_token].len,
                                                             tokens[*cur_token].str);

        cmd->args.reg = reg->num;
        cmd->byte.reg = true;

        *cur_token += 1;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm(String* tokens, size_t* cur_token, Cmd* cmd,
                                  const InputFileInfo* inp_file) {
    assert(cmd);
    assert(tokens);
    assert(inp_file);

    if (tokens[*cur_token].len == 0)
        return Status::NORMAL_WORK;

    STATUS_CHECK(asm_read_arg_imm_double(tokens, cur_token, cmd, inp_file));
    STATUS_CHECK(asm_read_arg_imm_int   (tokens, cur_token, cmd, inp_file));

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm_int(String* tokens, size_t* cur_token, Cmd* cmd,
                                      const InputFileInfo* inp_file) {
    assert(cmd);
    assert(tokens);
    assert(inp_file);

    if (!cmd->info->args.imm_int || tokens[*cur_token].len == 0)
        return Status::NORMAL_WORK;

    int sscanf_num = 0;
    int sscanf_res = String_sscanf(tokens[*cur_token], IMM_INT_T_PRINTF "%n",
                                   &cmd->args.imm_int, &sscanf_num);

    if (sscanf_res != 1 || sscanf_num != tokens[*cur_token].len)
        THROW_SYNTAX_ERROR_("Invalid immediate argument \"%.*s\"", tokens[*cur_token].len,
                                                                   tokens[*cur_token].str);

    cmd->byte.imm = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm_double(String* tokens, size_t* cur_token, Cmd* cmd,
                                         const InputFileInfo* inp_file) {
    assert(cmd);
    assert(tokens);
    assert(inp_file);

    if (!cmd->info->args.imm_double || tokens[*cur_token].len == 0)
        return Status::NORMAL_WORK;

    int sscanf_num = 0;
    int sscanf_res = String_sscanf(tokens[*cur_token], IMM_DOUBLE_T_PRINTF "%n",
                                   &cmd->args.imm_double, &sscanf_num);

    if (sscanf_res != 1 || sscanf_num != tokens[*cur_token].len)
        THROW_SYNTAX_ERROR_("Invalid immediate argument \"%.*s\"", tokens[*cur_token].len,
                                                                   tokens[*cur_token].str);

    cmd->byte.imm = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

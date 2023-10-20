#include "read.h"

#define THROW_SYNTAX_ERROR_(...)  return asm_throw_syntax_error(tokens[*cur_token], \
                                                                asm_info,           \
                                                                __VA_ARGS__)

Status::Statuses asm_read_args(String* tokens, size_t* cur_token, Cmd* cmd,
                               const AsmInfo* asm_info, JumpLabel* labels) {
    assert(tokens);
    assert(cur_token);
    assert(cmd);
    assert(asm_info);
    assert(labels);

    STATUS_CHECK(asm_read_args_ram      (tokens, cur_token, cmd, asm_info));

    STATUS_CHECK(asm_read_arg_reg       (tokens, cur_token, cmd, asm_info));
    STATUS_CHECK(asm_read_arg_imm_double(tokens, cur_token, cmd, asm_info));
    STATUS_CHECK(asm_read_arg_label     (tokens, cur_token, cmd, asm_info, labels));

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_args_ram(String* tokens, size_t* cur_token, Cmd* cmd,
                                   const AsmInfo* asm_info) {
    assert(tokens);
    assert(cur_token);
    assert(cmd);
    assert(asm_info);

    if (tokens[*cur_token].str == nullptr || tokens[*cur_token].len == 0
        || tokens[*cur_token].str[0] != '[')
            return Status::NORMAL_WORK;

    *cur_token += 1;

    if (!cmd->info->args.ram)
        THROW_SYNTAX_ERROR_("Command doesn't support ram args.");

    cmd->keys.ram = true;

    STATUS_CHECK(asm_read_arg_reg(tokens, cur_token, cmd, asm_info));

    if (tokens[*cur_token].str != nullptr && tokens[*cur_token].len != 0
        && tokens[*cur_token].str[0] != ']')
            STATUS_CHECK(asm_read_arg_imm_int(tokens, cur_token, cmd, asm_info));

    if (tokens[*cur_token].len == 0 || tokens[*cur_token].str[0] != ']')
        THROW_SYNTAX_ERROR_("Expected \"]\".");

    *cur_token += 1;

    if (tokens[*cur_token].len != 0)
        THROW_SYNTAX_ERROR_("No more arguments expected.");

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_reg(String* tokens, size_t* cur_token, Cmd* cmd,
                                  const AsmInfo* asm_info) {
    assert(cmd);
    assert(tokens);
    assert(asm_info);

    if (!cmd->info->args.reg || tokens[*cur_token].len == 0 || cmd->keys.reg)
        return Status::NORMAL_WORK;

    if (isalpha(tokens[*cur_token].str[0])) {
        const RegInfo* reg = find_reg_by_name(tokens[*cur_token]);

        if (reg == nullptr) {
            if (cmd->info->args.label) //< it may be label, not reg
                return Status::NORMAL_WORK;

            THROW_SYNTAX_ERROR_("Invalid reg name \"%.*s\"", String_PRINTF(tokens[*cur_token]));
        }

        cmd->args.reg = reg->num;

        cmd->keys.reg = true;
        *cur_token += 1;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm_int(String* tokens, size_t* cur_token, Cmd* cmd,
                                      const AsmInfo* asm_info) {
    assert(cmd);
    assert(tokens);
    assert(asm_info);

    if (!cmd->info->args.ram || tokens[*cur_token].len == 0 || cmd->keys.imm_int
        || !maybedigit(tokens[*cur_token].str[0]))
            return Status::NORMAL_WORK;

    int sscanf_num = 0;
    int sscanf_res = String_sscanf(tokens[*cur_token], IMM_INT_T_PRINTF "%n",
                                   &cmd->args.imm_int, &sscanf_num);

    if (sscanf_res != 1 || sscanf_num != tokens[*cur_token].len)
        THROW_SYNTAX_ERROR_("Invalid immediate argument \"%.*s\"",
                                String_PRINTF(tokens[*cur_token]));

    cmd->keys.imm_int = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm_double(String* tokens, size_t* cur_token, Cmd* cmd,
                                         const AsmInfo* asm_info) {
    assert(cmd);
    assert(tokens);
    assert(asm_info);

    if (!cmd->info->args.imm_double || tokens[*cur_token].len == 0 || cmd->keys.imm_double
        || !maybedigit(tokens[*cur_token].str[0]))
            return Status::NORMAL_WORK;

    int sscanf_num = 0;
    int sscanf_res = String_sscanf(tokens[*cur_token], IMM_DOUBLE_T_PRINTF "%n",
                                   &cmd->args.imm_double, &sscanf_num);

    if (sscanf_res != 1 || sscanf_num != tokens[*cur_token].len)
        THROW_SYNTAX_ERROR_("Invalid immediate argument \"%.*s\"",
                                String_PRINTF(tokens[*cur_token]));

    cmd->keys.imm_double = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_label(String* tokens, size_t* cur_token, Cmd* cmd,
                                    const AsmInfo* asm_info, JumpLabel* labels) {
    assert(tokens);
    assert(cur_token);
    assert(cmd);
    assert(asm_info);
    assert(labels);

    if (!cmd->info->args.label || tokens[*cur_token].len == 0 || cmd->keys.imm_int)
        return Status::NORMAL_WORK;

    STATUS_CHECK(asm_get_label_byte(labels, tokens[*cur_token], asm_info,
                                    &cmd->args.imm_int, asm_info->final_pass));

    cmd->keys.imm_int = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

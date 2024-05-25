#include "read.h"

#define THROW_SYNTAX_ERROR_(...)  return asm_throw_syntax_error(asm_data, line, __VA_ARGS__)

Status::Statuses asm_read_cmd(Asm* asm_data, AsmLine* line) {
    assert(asm_data);
    assert(line);

    if (line->tokens[line->cur_token].len == 0)
        return Status::NORMAL_WORK;

    line->cmd.info = find_command_by_name(line->tokens[line->cur_token]);
    if (line->cmd.info == nullptr) {

        if (!(line->tokens[line->cur_token + 1].len == 1 &&
              line->tokens[line->cur_token + 1].str[0] == ':')) { //< if not label
            THROW_SYNTAX_ERROR_("Command \"%.*s\" not found.",
                                String_PRINTF(line->tokens[line->cur_token]));
        }

        if (asm_data->pass_num == 0)
            STATUS_CHECK(asm_new_label(line->tokens[line->cur_token], (size_t)asm_data->buf.capacity,
                                       asm_data, line));

        line->cur_token += 2;

        if (line->tokens[line->cur_token].len != 0)
            THROW_SYNTAX_ERROR_("Unexpected arg given.");

        return Status::NORMAL_WORK;
    }

    line->cmd.keys.num = line->cmd.info->num & CMD_BYTE_NUM_BIT_MASK;

    line->cur_token += 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_args(const Asm* asm_data, AsmLine* line) {
    assert(asm_data);
    assert(line);

    STATUS_CHECK(asm_read_args_ram(asm_data, line));

    STATUS_CHECK(asm_read_arg_reg       (asm_data, line));
    STATUS_CHECK(asm_read_arg_imm_double(asm_data, line));
    STATUS_CHECK(asm_read_arg_label     (asm_data, line));

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_args_ram(const Asm* asm_data, AsmLine* line) {
    assert(asm_data);
    assert(line);

    String* tokens = line->tokens;
    size_t* cur_token = &line->cur_token;

    if (tokens[*cur_token].len == 0 || tokens[*cur_token].str[0] != '[')
            return Status::NORMAL_WORK;

    *cur_token += 1;

    if (!line->cmd.info->is_args.ram)
        THROW_SYNTAX_ERROR_("Command doesn't support ram args.");

    line->cmd.keys.ram = true;

    STATUS_CHECK(asm_read_arg_reg(asm_data, line));

    if (tokens[*cur_token].str != nullptr && tokens[*cur_token].len != 0 &&
        tokens[*cur_token].str[0] != ']')
            STATUS_CHECK(asm_read_arg_imm_int(asm_data, line));

    if (tokens[*cur_token].len == 0 || tokens[*cur_token].str[0] != ']')
        THROW_SYNTAX_ERROR_("Expected \"]\".");

    *cur_token += 1;

    if (tokens[*cur_token].len != 0)
        THROW_SYNTAX_ERROR_("No more arguments expected.");

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_reg(const Asm* asm_data, AsmLine* line) {
    assert(asm_data);
    assert(line);

    String* tokens = line->tokens;
    size_t* cur_token = &line->cur_token;

    if (tokens[*cur_token].len == 0 || !line->cmd.info ||
        !line->cmd.info->is_args.reg || line->cmd.keys.reg)
            return Status::NORMAL_WORK;

    if (isalpha(tokens[*cur_token].str[0])) {
        const RegInfo* reg = find_reg_by_name(tokens[*cur_token]);

        if (reg == nullptr) {
            if (line->cmd.info->is_args.label) //< it may be label, not reg
                return Status::NORMAL_WORK;

            THROW_SYNTAX_ERROR_("Invalid reg name \"%.*s\"",
                                String_PRINTF(tokens[*cur_token]));
        }

        line->cmd.args.reg = reg->num;

        line->cmd.keys.reg = true;
        *cur_token += 1;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm_int(const Asm* asm_data, AsmLine* line) {
    assert(asm_data);
    assert(line);

    String* tokens = line->tokens;
    size_t* cur_token = &line->cur_token;

    if (tokens[*cur_token].len == 0 || !line->cmd.info || !line->cmd.info->is_args.ram ||
        line->cmd.keys.imm_int || !maybedigit(tokens[*cur_token].str[0]))
            return Status::NORMAL_WORK;

    int sscanf_num = 0;
    int sscanf_res = String_sscanf(tokens[*cur_token], IMM_INT_T_PRINTF "%n",
                                   &line->cmd.args.imm_int, &sscanf_num);

    if (sscanf_res != 1 || sscanf_num != tokens[*cur_token].len)
        THROW_SYNTAX_ERROR_("Invalid immediate argument \"%.*s\"",
                                String_PRINTF(tokens[*cur_token]));

    line->cmd.keys.imm_int = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_imm_double(const Asm* asm_data, AsmLine* line) {
    assert(asm_data);
    assert(line);

    String* tokens = line->tokens;
    size_t* cur_token = &line->cur_token;

    if (tokens[*cur_token].len == 0 || !line->cmd.info || !line->cmd.info->is_args.imm_double ||
        line->cmd.keys.imm_double || !maybedigit(tokens[*cur_token].str[0]))
            return Status::NORMAL_WORK;

    bool is_exp = false;
    if (tokens[*cur_token].str[tokens[*cur_token].len - 1] == 'e') {
        is_exp = true;
        tokens[*cur_token].str[tokens[*cur_token].len] = '+';
        tokens[*cur_token].len += tokens[*cur_token + 1].len + 1;
    }

    int sscanf_num = 0;
    int sscanf_res = String_sscanf(tokens[*cur_token], IMM_DOUBLE_T_PRINTF "%n",
                                   &line->cmd.args.imm_double, &sscanf_num);

    if (sscanf_res != 1 || sscanf_num != tokens[*cur_token].len)
        THROW_SYNTAX_ERROR_("Invalid immediate argument \"%.*s\"",
                                String_PRINTF(tokens[*cur_token]));

    line->cmd.keys.imm_double = true;
    *cur_token += is_exp ? 2 : 1;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_read_arg_label(const Asm* asm_data, AsmLine* line) {
    assert(asm_data);
    assert(line);

    String* tokens = line->tokens;
    size_t* cur_token = &line->cur_token;

    if (tokens[*cur_token].len == 0 || !line->cmd.info ||
        !line->cmd.info->is_args.label || line->cmd.keys.imm_int)
            return Status::NORMAL_WORK;

    STATUS_CHECK(asm_get_label_addr(tokens[*cur_token], asm_data, line, &line->cmd.args.imm_int));

    line->cmd.keys.imm_int = true;
    *cur_token += 1;

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

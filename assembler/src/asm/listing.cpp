#include "listing.h"

#define F_PRINTF_CHECK_(printf)  if (printf == EOF) return Status::OUT_FILE_ERROR
#define F_WRITE_CHECK_(write)    if (!write)        return Status::OUT_FILE_ERROR

Status::Statuses asm_write_cmd_listing(FILE* file, const Asm* asm_data, const AsmLine* line) {
    assert(file);
    assert(asm_data);
    assert(line);

    F_PRINTF_CHECK_(file_printf(file, ";%4zu | %04zu |", line->num, asm_data->buf.size));

    if (line->cmd.info == nullptr) {
        F_PRINTF_CHECK_(file_printf(file, "%*s | ", cmd_listing_calc_tab(&line->cmd), ""));

        const_String text = String_skip_spaces(String_TO_const(line->text));
        if (text.len != 0)
            F_WRITE_CHECK_(file_printf(file, "%.*s", String_PRINTF(text)));

        F_PRINTF_CHECK_(file_printf(file, "\n"));

        return Status::NORMAL_WORK;
    }

    // Command in hex format

    const Cmd* cmd = &line->cmd;

    for (size_t i = 0; i < sizeof(cmd->keys); i++)
        F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->keys)[i]));

    if (cmd->keys.reg)
        for (size_t i = 0; i < sizeof(cmd->args.reg); i++)
            F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->args.reg)[i]));

    if (cmd->keys.imm_int)
        for (size_t i = 0; i < sizeof(cmd->args.imm_int); i++)
            F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->args.imm_int)[i]));

    if (cmd->keys.imm_double)
        for (size_t i = 0; i < sizeof(cmd->args.imm_double); i++)
            F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->args.imm_double)[i]));

    F_PRINTF_CHECK_(file_printf(file, "%*s", cmd_listing_calc_tab(cmd), ""));

    // Command in text format

    F_PRINTF_CHECK_(file_printf(file, " | %.*s", String_PRINTF(cmd->info->name)));

    if (cmd->keys.ram) {
        F_PRINTF_CHECK_(file_printf(file, " ["));
    } else if (cmd_is_any_arg_given(cmd)) {
        F_PRINTF_CHECK_(file_printf(file, " "));
    }

    if (cmd->keys.reg) {
        const_String reg_name = find_reg_by_num(cmd->args.reg)->name;
        F_PRINTF_CHECK_(file_printf(file, "%.*s", String_PRINTF(reg_name)));

        if (cmd->keys.imm_int || cmd->keys.imm_double)
            F_PRINTF_CHECK_(file_printf(file, "+"));
    }

    if (cmd->keys.imm_double) {
        F_PRINTF_CHECK_(file_printf(file, IMM_DOUBLE_T_PRINTF, cmd->args.imm_double));

        if (cmd->keys.imm_int)
            F_PRINTF_CHECK_(file_printf(file, "+"));
    }

    if (cmd->keys.imm_int) {
        if (cmd->info->is_args.label && !cmd->keys.ram) {
            const_String label_name = asm_get_label_name(asm_data->labels, cmd->args.imm_int);
            F_PRINTF_CHECK_(file_printf(file, "%.*s (" IMM_INT_T_PRINTF ")",
                                String_PRINTF(label_name), cmd->args.imm_int));
        } else
            F_PRINTF_CHECK_(file_printf(file, IMM_INT_T_PRINTF, cmd->args.imm_int));
    }

    if (cmd->keys.ram)
        F_PRINTF_CHECK_(file_printf(file, "]"));

    F_WRITE_CHECK_(asm_write_listing_comment(file, line->comment));

    F_PRINTF_CHECK_(file_printf(file, "\n"));

    return Status::NORMAL_WORK;
}

Status::Statuses asm_write_header_listing(FILE* listing_file) {
    assert(listing_file);

    static_assert(sizeof(FILE_HEADER.sign) == 2);

    F_PRINTF_CHECK_(file_printf(listing_file, ";Signature: %c%c\n"
                                              ";Version: %d\n",
                                FILE_HEADER.sign, FILE_HEADER.sign >> 8, FILE_HEADER.version));

    return Status::NORMAL_WORK;
}
#undef F_PRINTF_CHECK_
#undef F_WRITE_CHECK_


bool asm_write_listing_comment(FILE* file, const char* comment) {
    if (comment == nullptr)
        return true;

    if (file_printf(file, " %s", comment) == EOF)
        return false;

    return true;
}

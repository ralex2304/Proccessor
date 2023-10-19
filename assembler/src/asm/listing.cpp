#include "listing.h"

#define F_PRINTF_CHECK_(printf)  if (printf == EOF) return Status::OUT_FILE_ERROR
#define F_WRITE_CHECK_(write)    if (!write)        return Status::OUT_FILE_ERROR

Status::Statuses asm_write_cmd_listing(FILE* file, const Cmd* cmd, JumpLabel* labels,
                                       const InputFileInfo* inp_file, const size_t binary_pos) {
    assert(file);
    assert(labels);
    assert(inp_file);
    // cmd assertion is not needed

    F_PRINTF_CHECK_(file_printf(file, ";%4zu - %4zu -", inp_file->line_num, binary_pos));

    if (cmd == nullptr) {
        F_PRINTF_CHECK_(file_printf(file, "%*s -", asm_listing_calc_tab(cmd), ""));
        F_WRITE_CHECK_(asm_write_listing_comment(file, inp_file->comment));
        F_PRINTF_CHECK_(file_printf(file, "\n"));

        return Status::NORMAL_WORK;
    }

    // Command in hex format

    F_PRINTF_CHECK_(file_printf(file, " %02X", cmd->byte));

    if (cmd->byte.reg)
        for (size_t i = 0; i < sizeof(cmd->args.reg); i++)
            F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->args.reg)[i]));

    if (cmd->byte.imm) {
        if (cmd->byte.ram || cmd->info->args.label) {
            for (size_t i = 0; i < sizeof(cmd->args.imm_int); i++)
                F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->args.imm_int)[i]));
        } else {
            for (size_t i = 0; i < sizeof(cmd->args.imm_double); i++)
                F_PRINTF_CHECK_(file_printf(file, " %02hhX", ((const char*)&cmd->args.imm_double)[i]));
        }
    }

    F_PRINTF_CHECK_(file_printf(file, "%*s", asm_listing_calc_tab(cmd), ""));

    // Command in text format

    F_PRINTF_CHECK_(file_printf(file, " - %.*s", cmd->info->name.len, cmd->info->name.str));

    if (cmd->info->args.ram && cmd->byte.ram) {
        F_PRINTF_CHECK_(file_printf(file, " ["));
    } else if (cmd->byte.imm || cmd->byte.reg) {
        F_PRINTF_CHECK_(file_printf(file, " "));
    }

    if (cmd->byte.reg) {
        c_String reg_name = find_reg_by_num(cmd->args.reg)->name;
        F_PRINTF_CHECK_(file_printf(file, "%.*s", reg_name.len, reg_name.str));

        if (cmd->byte.imm)
            F_PRINTF_CHECK_(file_printf(file, "+"));
    }

    if (cmd->byte.imm) {
        if (cmd->byte.ram) {
            F_PRINTF_CHECK_(file_printf(file, IMM_INT_T_PRINTF, cmd->args.imm_int));
        } else if (cmd->info->args.label) {
            c_String label_name = asm_get_label_name(labels, cmd->args.imm_int);
            F_PRINTF_CHECK_(file_printf(file, "%.*s (" IMM_INT_T_PRINTF ")",
                                (int)label_name.len, label_name.str, cmd->args.imm_int));
        } else {
            F_PRINTF_CHECK_(file_printf(file, IMM_DOUBLE_T_PRINTF, cmd->args.imm_double));
        }
    }

    if (cmd->info->args.ram && cmd->byte.ram)
        F_PRINTF_CHECK_(file_printf(file, "]"));

    F_WRITE_CHECK_(asm_write_listing_comment(file, inp_file->comment));

    F_PRINTF_CHECK_(file_printf(file, "\n"));

    return Status::NORMAL_WORK;
}

Status::Statuses asm_write_header_listing(FILE* listing_file) {
    assert(listing_file);

    static_assert(sizeof(FILE_HEADER.sign) == 2);

    F_PRINTF_CHECK_(file_printf(listing_file, ";Signature: %c%c\n"
                                              ";Version: %d\n",
                                FILE_HEADER.sign[0], FILE_HEADER.sign[1], FILE_HEADER.version));

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

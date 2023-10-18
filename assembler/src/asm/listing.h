#ifndef ASM_LISTING_H_
#define ASM_LISTING_H_

#include <stdlib.h>
#include <stdio.h>

#include "lib/utils/statuses.h"
#include "cmd.h"
#include "lib/utils/macros.h"
#include "lib/file/file.h"
#include "jump.h"

/**
 * @brief Writes cmd to listing
 *
 * @param file
 * @param cmd
 * @param labels
 * @param binary_pos
 * @param line_num
 * @param comment_symb
 * @param comment
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd_listing(FILE* file, const Cmd* cmd, JumpLabel* labels,
                                       const size_t binary_pos, const size_t line_num,
                                       const char comment_symb, const char* comment);

/**
 * @brief Writes comment to listing file
 *
 * @param file
 * @param comment_symb
 * @param comment
 * @return true
 * @return false
 */
bool asm_write_listing_comment(FILE* file, const char comment_symb, const char* comment);

/**
 * @brief Writes header to listing
 *
 * @param listing_file
 * @return Status::Statuses
 */
Status::Statuses asm_write_header_listing(FILE* listing_file);

/**
 * @brief Calculates tab for listing
 *
 * @param cmd
 * @return size_t
 */
inline size_t asm_listing_calc_tab(const Cmd* cmd) {
    size_t res = 0;

    if (cmd == nullptr)
        res += sizeof(cmd->byte) * 3;

    if (cmd == nullptr || !cmd->byte.reg)
        res += sizeof(cmd->args.reg) * 3;

    if (cmd == nullptr || !cmd->byte.imm)
        res += MAX(sizeof(cmd->args.imm_double), sizeof(cmd->args.imm_int)) * 3;

    if (cmd != nullptr && cmd->byte.imm) {
        if (cmd->info->args.label || cmd->byte.ram)
            res += MAX(0, (ssize_t)sizeof(cmd->args.imm_double) - (ssize_t)sizeof(cmd->args.imm_int));
        else
            res += MAX(0, (ssize_t)sizeof(cmd->args.imm_int) - (ssize_t)sizeof(cmd->args.imm_double));
    }

    return res;
}

#endif //< #ifndef ASM_LISTING_H_

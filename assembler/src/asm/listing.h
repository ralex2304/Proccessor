#ifndef ASM_LISTING_H_
#define ASM_LISTING_H_

#include <stdlib.h>
#include <stdio.h>

#include "lib/utils/statuses.h"
#include "cmd.h"
#include "lib/utils/macros.h"
#include "lib/file/file.h"
#include "jump.h"
#include "errors.h"

/**
 * @brief Writes command to listing
 *
 * @param file
 * @param cmd
 * @param labels
 * @param inp_file
 * @param binary_pos
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd_listing(FILE* file, const Cmd* cmd, JumpLabel* labels,
                                       const AsmInfo* inp_file, const size_t binary_pos);

/**
 * @brief Writes comment to listing file
 *
 * @param file
 * @param comment
 * @return true
 * @return false
 */
bool asm_write_listing_comment(FILE* file, const char* comment);

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
        res += sizeof(cmd->keys) * 3;

    if (cmd == nullptr || !cmd->keys.reg)
        res += sizeof(cmd->args.reg) * 3;

    if (cmd == nullptr || !cmd->keys.imm_int)
        res += sizeof(cmd->args.imm_int) * 3;

    if (cmd == nullptr || !cmd->keys.imm_double)
        res += sizeof(cmd->args.imm_double) * 3;

    return res;
}

#endif //< #ifndef ASM_LISTING_H_

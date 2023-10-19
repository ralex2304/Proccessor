#ifndef ASM_WRITE_H_
#define ASM_WRITE_H_

#include <stdlib.h>
#include <stdio.h>

#include "lib/utils/buf_lib.h"
#include "lib/utils/statuses.h"
#include "listing.h"
#include "errors.h"

/**
 * @brief Writes one cmd to buf
 *
 * @param buf
 * @param cmd
 * @param labels
 * @param inp_file
 * @param listing_file
 * @param final_pass
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd(Buffer* buf, const Cmd* cmd, JumpLabel* labels,
                               const InputFileInfo* inp_file,
                               FILE* listing_file, const bool final_pass);

/**
 * @brief Writes one cmd to bin file
 *
 * @param buf
 * @param cmd
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd_bin(Buffer* buf, const Cmd* cmd);

/**
 * @brief Writes header
 *
 * @param buf
 * @param listing_file
 * @param first_pass
 * @return Status::Statuses
 */
Status::Statuses asm_write_header(Buffer* buf, FILE* listing_file, const bool first_pass);

/**
 * @brief Writes header to bin buffer
 *
 * @param buf
 * @return Status::Statuses
 */
Status::Statuses asm_write_header_bin(Buffer* buf);

/**
 * @brief Calculates command size in bytes
 *
 * @param cmd
 * @return size_t
 */
inline size_t asm_calc_cmd_size(const Cmd* cmd) {
    return sizeof(cmd->byte) + cmd->byte.reg * sizeof(cmd->args.reg)
                             + cmd->byte.imm * ((cmd->byte.ram || cmd->info->args.label)
                                                ? sizeof(cmd->args.imm_int)
                                                : sizeof(cmd->args.imm_double));
}

#endif //< #ifndef ASM_WRITE_H_

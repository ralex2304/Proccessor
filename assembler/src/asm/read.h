#ifndef ASM_READ_H_
#define ASM_READ_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include "lib/utils/statuses.h"
#include "lib/text/text_lib.h"
#include "cmd.h"
#include "jump.h"

/**
 * @brief Reads command args
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param asm_info
 * @param labels
 * @return Status::Statuses
 */
Status::Statuses asm_read_args(String* tokens, size_t* cur_token, Cmd* cmd,
                               const AsmInfo* asm_info, JumpLabel* labels);

/**
 * @brief Reads ram arguments
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param asm_info
 * @return Status::Statuses
 */
Status::Statuses asm_read_args_ram(String* tokens, size_t* cur_token, Cmd* cmd,
                                   const AsmInfo* asm_info);

/**
 * @brief Reads reg argument
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param asm_info
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_reg(String* tokens, size_t* cur_token, Cmd* cmd,
                                  const AsmInfo* asm_info);

Status::Statuses asm_read_arg_label(String* tokens, size_t* cur_token, Cmd* cmd,
                                    const AsmInfo* asm_info, JumpLabel* labels);

/**
 * @brief Reads integer immutable argument
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param asm_info
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_imm_int(String* tokens, size_t* cur_token, Cmd* cmd,
                                      const AsmInfo* asm_info);

/**
 * @brief Reads double immutable argument
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param asm_info
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_imm_double(String* tokens, size_t* cur_token, Cmd* cmd,
                                         const AsmInfo* asm_info);

#endif //< #ifndef ASM_READ_H_

#ifndef ASM_READ_H_
#define ASM_READ_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include "lib/utils/statuses.h"
#include "cmd.h"
#include "jump.h"

/**
 * @brief Reads command args from tokens
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param labels
 * @param line_num
 * @param first_pass
 * @return Status::Statuses
 */
Status::Statuses asm_read_args(const char** tokens, size_t* cur_token, Cmd* cmd,
                               JumpLabel* labels, const size_t line_num, const bool first_pass);

/**
 * @brief Reads ram arguments
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_args_ram(const char** tokens, size_t* cur_token, Cmd* cmd, const size_t line_num);

/**
 * @brief Reads reg argument
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_reg(const char** tokens, size_t* cur_token,
                                  Cmd* cmd, const size_t line_num);

/**
 * @brief Reads immutable args
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_imm(const char** tokens, size_t* cur_token,
                                  Cmd* cmd, const size_t line_num);

/**
 * @brief Reads integer immutable argument
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_imm_int(const char** tokens, size_t* cur_token,
                                      Cmd* cmd, const size_t line_num);

/**
 * @brief Reads double immutable argument
 *
 * @param tokens
 * @param cur_token
 * @param cmd
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_imm_double(const char** tokens, size_t* cur_token,
                                         Cmd* cmd, const size_t line_num);

#endif //< #ifndef ASM_READ_H_

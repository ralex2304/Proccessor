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
#include "asm.h"

/**
 * @brief Reads command
 *
 * @param asm_data
 * @param line
 * @return Status::Statuses
 */
Status::Statuses asm_read_cmd(Asm* asm_data, AsmLine* line);

/**
 * @brief Reads command args
 *
 * @param asm_data
 * @param line
 * @return Status::Statuses
 */
Status::Statuses asm_read_args(const Asm* asm_data, AsmLine* line);

/**
 * @brief Reads ram arguments
 *
 * @param asm_data
 * @param line
 * @return Status::Statuses
 */
Status::Statuses asm_read_args_ram(const Asm* asm_data, AsmLine* line);

/**
 * @brief Reads reg argument
 *
 * @param asm_data
 * @param line
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_reg(const Asm* asm_data, AsmLine* line);

/**
 * @brief Reads label
 *
 * @param asm_data
 * @param line
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_label(const Asm* asm_data, AsmLine* line);

/**
 * @brief Reads integer immutable argument
 *
 * @param asm_data
 * @param line
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_imm_int(const Asm* asm_data, AsmLine* line);

/**
 * @brief Reads double immutable argument
 *
 * @param asm_data
 * @param line
 * @return Status::Statuses
 */
Status::Statuses asm_read_arg_imm_double(const Asm* asm_data, AsmLine* line);

#endif //< #ifndef ASM_READ_H_

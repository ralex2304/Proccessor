#ifndef ASM_JUMP_H_
#define ASM_JUMP_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "lib/utils/statuses.h"
#include "cmd.h"
#include "errors.h"
#include "asm.h"

/**
 * @brief Adds new label to labels
 *
 * @param name
 * @param addr
 * @param asm_data
 * @param line
 * @return Status::Statuses
 */
Status::Statuses asm_new_label(const String name, const size_t addr,
                               Asm* asm_data, const AsmLine* line);

/**
 * @brief Gets label addr by name
 *
 * @param name
 * @param asm_data
 * @param line
 * @param addr
 * @return Status::Statuses
 */
Status::Statuses asm_get_label_addr(const String name, const Asm* asm_data,
                                    const AsmLine* line, Imm_int_t* addr);

/**
 * @brief Returns label name by addr
 *
 * @param asm_data
 * @param addr
 * @return const_String
 */
const_String asm_get_label_name(const Asm* asm_data, const Imm_int_t addr);

#endif //< #ifndef ASM_JUM_H_

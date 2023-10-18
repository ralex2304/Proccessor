#ifndef ASM_JUMP_H_
#define ASM_JUMP_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "lib/utils/statuses.h"
#include "cmd.h"

/**
 * @brief Specifies jump cmd label data
 */
struct JumpLabel {
    const char* name = nullptr;
    ssize_t byte = -1;
};

/**
 * @brief Puts new label to labels array
 *
 * @param labels
 * @param name
 * @param byte
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_new_label(JumpLabel* labels, const char* name,
                               const size_t byte, const size_t line_num);

/**
 * @brief Gets label from labels array
 *
 * @param labels
 * @param name
 * @param byte
 * @param line_num
 * @param first_pass
 * @return Status::Statuses
 */
Status::Statuses asm_get_label(JumpLabel* labels, const char* name,
                               Imm_int_t* byte, const size_t line_num,
                               const bool first_pass);


#endif //< #ifndef ASM_JUM_H_

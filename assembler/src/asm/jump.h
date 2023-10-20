#ifndef ASM_JUMP_H_
#define ASM_JUMP_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "lib/utils/statuses.h"
#include "cmd.h"
#include "errors.h"

/**
 * @brief Specifies jump cmd label data
 */
struct JumpLabel {
    String name = {nullptr, 0};
    ssize_t byte = -1;
};

/**
 * @brief Adds new label to labels
 *
 * @param labels
 * @param name
 * @param inp_file
 * @param byte
 * @return Status::Statuses
 */
Status::Statuses asm_new_label(JumpLabel* labels, const String name,
                               const AsmInfo* inp_file, const size_t byte);

/**
 * @brief Gets label byte by name
 *
 * @param labels
 * @param name
 * @param inp_file
 * @param byte
 * @param final_pass
 * @return Status::Statuses
 */
Status::Statuses asm_get_label_byte(JumpLabel* labels, const String name,
                                    const AsmInfo* inp_file, Imm_int_t* byte,
                                    const bool final_pass);

/**
 * @brief Returns label name by byte
 *
 * @param lables
 * @param byte
 * @return c_String
 */
c_String asm_get_label_name(const JumpLabel* lables, const Imm_int_t byte);

#endif //< #ifndef ASM_JUM_H_

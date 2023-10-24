#ifndef ASM_ERRORS_
#define ASM_ERRORS_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#include "lib/utils/statuses.h"
#include "lib/text/text_lib.h"
#include "lib/utils/console.h"
#include "cmd.h"
#include "asm.h"

#ifdef __GNUG__ //< because of __attribute__
/**
 * @brief Throws syntax error
 *
 * @param asm_data
 * @param line
 * @param err_msg
 * @param ...
 * @return Status::Statuses always SYNTAX_ERROR
 */
Status::Statuses asm_throw_syntax_error(const Asm* asm_data, const AsmLine* line,
                                        const char* err_msg, ...)
                                        __attribute__ ((format(printf, 3, 4)));
#else //< #ifndef __GNUG__
/**
 * @brief Throws syntax error
 *
 * @param asm_data
 * @param line
 * @param err_msg
 * @param ...
 * @return Status::Statuses always SYNTAX_ERROR
 */
Status::Statuses asm_throw_syntax_error(const Asm* asm_data, const AsmLine* line,
                                        const char* err_msg, ...);
#endif //< #ifdef __GNUG__

/**
 * @brief Checks if cmd requires at least one argument
 *
 * @param cmd
 * @return true
 * @return false
 */
inline bool cmd_is_arg_required(const Cmd* cmd) {
    assert(cmd);

    if (cmd->info == nullptr)
        return false;

    return cmd->info->is_args.label || cmd->info->is_args.imm_int || cmd->info->is_args.imm_double
        || cmd->info->is_args.reg; //< ram check is not needed
}

/**
 * @brief Checks if at least one argument was given to a function
 *
 * @param cmd
 * @return true
 * @return false
 */
inline bool cmd_is_any_arg_given(const Cmd* cmd) {
    return cmd->keys.imm_int || cmd->keys.imm_double || cmd->keys.reg; //< ram check is not needed
}

#endif // #ifndef ASM_ERRORS_

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

/**
 * @brief Info about assembling
 */
struct AsmInfo {
    String line = {};               //< current line text
    size_t line_num = 0;            //< current line number
    const char* comment = nullptr;  //< current line comment
    const char* filename = nullptr; //< input filename
    bool final_pass = false;        //< enables some final checkups for final asm pass
};

/**
 * @brief Throws syntax error
 *
 * @param token current command token
 * @param file_info
 * @param err_msg
 * @param ...
 * @return Status::Statuses always SYNTAX_ERROR
 */
Status::Statuses asm_throw_syntax_error(const String token, const AsmInfo* file_info,
                                        const char* err_msg, ...);

/**
 * @brief Checks if cmd requires at least one argument
 *
 * @param cmd
 * @return true
 * @return false
 */
inline bool asm_is_arg_required(const Cmd* cmd) {
    return cmd->info->args.label || cmd->info->args.imm_int || cmd->info->args.imm_double
        || cmd->info->args.reg; ///< ram check is not needed
}

/**
 * @brief Checks if at least one argument was given to a function
 *
 * @param cmd
 * @return true
 * @return false
 */
inline bool asm_is_any_arg_given(const Cmd* cmd) {
    return cmd->keys.imm_int || cmd->keys.imm_double || cmd->keys.reg; //< ram check is not needed
}

#endif // #ifndef ASM_ERRORS_

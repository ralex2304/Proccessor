#ifndef ASM_ERRORS_
#define ASM_ERRORS_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#include "lib/utils/statuses.h"
#include "lib/text/text_lib.h"
#include "lib/utils/console.h"

/**
 * @brief Debug info about input file
 */
struct InputFileInfo {
    String line = {};               //< current line text
    size_t line_num = 0;            //< current line number
    const char* comment = nullptr;  //< current line comment
    const char* filename = nullptr; //< input filename
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
Status::Statuses asm_throw_syntax_error(const String token, const InputFileInfo* file_info,
                                        const char* err_msg, ...);

#endif // #ifndef ASM_ERRORS_

#include "errors.h"

Status::Statuses asm_throw_syntax_error(const String token, const AsmInfo* asm_info,
                                        const char* err_msg, ...) {
    assert(err_msg);
    assert(asm_info->line.len == 0 || asm_info->line.str);
    assert(         token.len == 0 ||          token.str);
    assert(asm_info->filename);

    const int token_begin = (token.str == nullptr) ? (int)asm_info->line.len
                                                   : (int)(token.str - asm_info->line.str);
    const int token_end   = (token.str == nullptr) ? (int)asm_info->line.len
                                                   : (int)(token_begin + token.len);

    fprintf(stderr, CONSOLE_STYLE(STYLE_BOLD, "%s:%zu:%d "
                                  COLOR_RED "syntax error: "), asm_info->filename,
            asm_info->line_num, token_begin + 1);

    va_list arg_list = {};
    va_start(arg_list, err_msg);
    vfprintf(stderr, err_msg, arg_list);
    va_end(arg_list);
    fprintf(stderr, "\n");

    fprintf(stderr, "%5zu | ", asm_info->line_num);

    fprintf(stderr, "%.*s", token_begin, asm_info->line.str);
    fprintf(stderr, CONSOLE_STYLE(STYLE_BOLD COLOR_RED, "%.*s"), String_PRINTF(token));
    fprintf(stderr, "%.*s\n", (int)(asm_info->line.len - token_end),
                                    asm_info->line.str + token_end);

    fprintf(stderr, "%5s | ", "");

    fprintf(stderr, "%*s", token_begin, "");
    fprintf(stderr, COLOR_RED STYLE_BOLD "^");
    for (ssize_t i = 0; i < token_end - token_begin - 1; i++)
        fprintf(stderr, "~");
    fprintf(stderr, STYLE_RESET "\n");

    return Status::SYNTAX_ERROR;
}

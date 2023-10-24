#include "errors.h"

Status::Statuses asm_throw_syntax_error(const Asm* asm_data, const AsmLine* line,
                                        const char* err_msg, ...) {
    assert(asm_data);
    assert(line);
    assert(err_msg);

    const String err_token = line->tokens[line->cur_token];

    const int token_begin = (err_token.str == nullptr) ? (int)line->text.len
                                                       : (int)(err_token.str - line->text.str);

    const int token_end   = (err_token.str == nullptr) ? (int)line->text.len
                                                       : (int)(token_begin + err_token.len);

    fprintf(stderr, CONSOLE_STYLE(STYLE_BOLD, "%s:%zu:%d "
                                  COLOR_RED "syntax error: "),
                    asm_data->files.inp_name, line->num, token_begin + 1);

    va_list arg_list = {};
    va_start(arg_list, err_msg);
    vfprintf(stderr, err_msg, arg_list);
    va_end(arg_list);
    fprintf(stderr, "\n");

    fprintf(stderr, "%5zu | ", line->num);

    fprintf(stderr, "%.*s", token_begin, line->text.str);
    fprintf(stderr, CONSOLE_STYLE(STYLE_BOLD COLOR_RED, "%.*s"), String_PRINTF(err_token));
    fprintf(stderr, "%.*s\n", (int)(line->text.len - token_end), line->text.str + token_end);

    fprintf(stderr, "%5s | ", "");

    fprintf(stderr, "%*s", token_begin, "");
    fprintf(stderr, COLOR_RED STYLE_BOLD "^");
    for (ssize_t i = 0; i < token_end - token_begin - 1; i++)
        fprintf(stderr, "~");
    fprintf(stderr, STYLE_RESET "\n");

    return Status::SYNTAX_ERROR;
}

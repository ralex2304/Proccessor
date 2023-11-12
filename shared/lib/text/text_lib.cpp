#include "text_lib.h"

String String_strtok_line(char* input) {
    static char* str = nullptr;

    if (input != nullptr)
        str = input;
    else
        input = str;

    if (str == nullptr)
        return String {};

    String ret = {};
    ret.str = str;

    while (*str && *str != '\r' && *str != '\n')
        str++;

    while (*str && (*str == '\r')) {
        if (ret.len == 0)
            ret.len = str - input;

        *str = '\0';
        str++;
    }
    if (ret.len == 0)
        ret.len = str - input;

    if (!*str) {
        str = nullptr;
        return ret;
    }

    *str = '\0';
    str++;

    if (*str == '\0')
        str = nullptr;

    return ret;
}

#define IS_TERMINATOR_(c_) (strchr(terminators, c_) != nullptr)

String String_strtok_special(char* input, const char* delim, const char* special_delim,
                             const char* terminators) {
    static char* str = nullptr;
    static String special = {nullptr, 0};

    if (input != nullptr) {
        str = input;
        special = {nullptr, 0};
    }

    if (special.str != nullptr) {
        String ret = special;
        special  = {nullptr, 0};
        return ret;
    }

    if (str == nullptr)
        return {nullptr, 0};

    while (!IS_TERMINATOR_(*str) && strchr(delim, *str) != nullptr)
        str++;

    String ret = {str, 0};

    if (!IS_TERMINATOR_(*str) && strchr(special_delim, *str) != nullptr) {
        ret = {str, 1};
        str++;
    } else {
        while (!IS_TERMINATOR_(*str) && strchr(delim, *str) == nullptr) {
            if (strchr(special_delim, *str) != nullptr) {
                special = {str, 1};
                ret.len = str - ret.str;
                str++;
                break;
            }

            str++;
        }
    }

    while (!IS_TERMINATOR_(*str) && strchr(delim, *str) != nullptr) {
        if (ret.len == 0)
            ret.len = str - ret.str;

        str++;
    }

    if (IS_TERMINATOR_(*str)) {
        if (ret.len == 0)
            ret.len = str - ret.str;
        str = nullptr;
    }

    return ret;
}
#undef IS_TERMINATOR_

void split_text_to_lines(char* text, String* lines_array) {
    assert(text);
    assert(lines_array);

    lines_array[0] = String_strtok_line(text);

    for (size_t i = 1; lines_array[i - 1].str != nullptr; i++) {
        lines_array[i] = String_strtok_line(nullptr);
    }
}

void split_line_with_special_delims(char* str, const char* delims, const char* special_delims,
                                    const char* terminators,
                                    String* array, const size_t array_max_size) {
    assert(str);
    assert(delims);
    assert(special_delims);
    assert(array);
    assert(array_max_size > 0);

    array[0] = String_strtok_special(str, delims, special_delims, terminators);
    for (size_t i = 1; i < array_max_size && array[i - 1].str != nullptr; i++) {
        array[i] = String_strtok_special(nullptr, delims, special_delims, terminators);
    }
}

int String_sscanf(String token, const char* format, ...) {
    assert(format);
    assert(token.str);

    const char tmp = token.str[token.len];
    token.str[token.len] = '\0';

    va_list arg_list = {};
    va_start(arg_list, format);
    int ret = vsscanf(token.str, format, arg_list);
    va_end(arg_list);

    token.str[token.len] = tmp;

    return ret;
}

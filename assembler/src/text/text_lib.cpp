#include "text_lib.h"

String string_strtok_line(char* input) {
    static char* str = nullptr;

    if (input != nullptr)
        str = input;
    else
        input = str;

    if (str == nullptr)
        return String {};

    String ret = {};
    ret.str = str;

    while (*str != '\r' && *str != '\n')
        str++;

    while ((*str == '\r') && *str) {
        if (ret.len == 0)
            ret.len = str - input;

        *str = '\0';
        str++;
    }
    if (ret.len == 0)
        ret.len = str - input;

    *str = '\0';
    str++;

    if (*str == '\0')
        str = nullptr;

    return ret;
}

const char* strtok_special(char* input, const char* delim, const char* special_delim) {
    static char* str = nullptr;
    static const char* special = nullptr;

    if (input != nullptr) {
        str = input;
        special = nullptr;
    }

    if (special != nullptr) {
        const char* ret = special;
        special = nullptr;
        return ret;
    }

    if (str == nullptr)
        return nullptr;

    while (*str && strchr(delim, *str) != nullptr) {
        str++;
    }

    const char* ret = str;

    const char* special_found = strchr(special_delim, *str);
    if (*str && special_found != nullptr) {
        *str = '\0';
        str++;

        ret = special_found;
    } else {
        while (*str && strchr(delim, *str) == nullptr) {
            special_found = strchr(special_delim, *str);
            if (special_found != nullptr) {
                special = special_found;
                *str = '\0';
                str++;
                break;
            }

            str++;
        }
    }

    while (*str && strchr(delim, *str) != nullptr) {
        *str = '\0';
        str++;
    }

    if (*str == '\0')
        str = nullptr;

    return ret;
}


void split_text_to_lines(char* text, String* lines_array) {
    assert(text);
    assert(lines_array);

    lines_array[0] = string_strtok_line(text);

    for (size_t i = 1; lines_array[i - 1].str != nullptr; i++) {
        lines_array[i] = string_strtok_line(nullptr);
    }
}

void split_line_with_special_delims(char* str, const char* delims, const char* special_delims,
                                    const char** array, const size_t array_max_size) {
    assert(str);
    assert(delims);
    assert(special_delims);
    assert(array);
    assert(array_max_size > 0);

    array[0] = strtok_special(str, delims, special_delims);
    for (size_t i = 1; i < array_max_size && array[i - 1] != nullptr; i++) {
        array[i] = strtok_special(nullptr, delims, special_delims);
    }
}


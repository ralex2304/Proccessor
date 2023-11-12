#ifndef TEXT_LIB_H_
#define TEXT_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/**
 * @brief Struct to keep const string and len together
 */
struct const_String {
    const char* str = nullptr; ///< const string
    ssize_t len = 0;     ///< string len
};

/**
 * @brief Struct to keep string and len together
 */
struct String {
    char* str = nullptr; ///< string
    ssize_t len = 0;     ///< string len
};

/**
 * @brief Macros for defining const strings
 *
 * @param str_
 */
#define String_CONST(str_) {str_, sizeof(str_) - 1}

/**
 * @brief String to const_String
 *
 * @param str_
 */
#define String_TO_const(str_) {(str_).str, (str_).len}

/**
 * @brief Macros for printing String (specifier "%.*s")
 *
 * @param str_
 */
#define String_PRINTF(str_) (int)(str_).len, (str_).str

/**
 * @brief Tokenizator for lines
 *
 * @param str
 * @return String
 */
String String_strtok_line(char* str);

/**
 * @brief Works as strtok, but also finds special_delim.
 * If found, puts it in found_special and quits
 *
 * @param input
 * @param delim
 * @param special_delim
 * @param terminators
 * @return String
 */
String String_strtok_special(char* input, const char* delim, const char* special_delim,
                             const char* terminators);

/**
 * @brief Uses my_strtok_lines and writes results into array
 *
 * @param text
 * @param lines_array
 */
void split_text_to_lines(char* text, String* lines_array);

/**
 * @brief Splits text to array of tokens. If founds special delim, treats it as single token
 *
 * @param str
 * @param delims
 * @param special_delims
 * @param terminators
 * @param array
 * @param array_max_size
 */
void split_line_with_special_delims(char* str, const char* delims, const char* special_delims,
                                    const char* terminators,
                                    String* array, const size_t array_max_size);
/**
 * @brief Counts ch in str
 *
 * @param str
 * @param ch
 * @return size_t
 */
inline size_t cntchar(const char* str, const char ch) {
    assert(str);

    size_t cnt = 0;

    for (size_t i = 0; str[i] != '\0'; i++)
        if (str[i] == ch)
            cnt++;

    return cnt;
}

/**
 * @brief Counts line number in str
 *
 * @param str
 * @param len
 * @return size_t
 */
inline size_t cntlines(const char* str, const size_t len) {
    assert(str);

    size_t cnt = cntchar(str, '\n');
    if (str[len - 1] != '\n')
        cnt++;

    return cnt;
}

/**
 * @brief Finds symb in str and replaces it with '\0'
 *
 * @param str
 * @param symb
 * @return returns pointer to the comment beginning
 */
inline const char* text_throw_out_comment(char* str, const char symb = ';') {
    assert(str);

    char* comment_beginning = strchr(str, symb);
    if   (comment_beginning != nullptr)
         *comment_beginning = '\0';

    return comment_beginning + (comment_beginning == nullptr ? 0 : 1);
}

/**
 * @brief Returns pointer to first non space symbol or '\0'
 *
 * @param str
 * @return const char*
 */
inline const char* str_skip_spaces(const char* str) {
    if (str == nullptr)
        return nullptr;

    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

/**
 * @brief Returns substring without spaces at the beginning
 *
 * @param s
 * @return const_String
 */
inline const_String String_skip_spaces(const_String s) {
    assert(s.len == 0 || s.str != nullptr);

    while (s.len > 0 && isspace(*s.str)) {
        s.str++;
        s.len--;
    }
    return s;
}

/**
 * @brief strcmp for const_String (not null-terminated strings)
 *
 * @param s1
 * @param s2
 * @return int
 */
inline int String_strcmp(const_String s1, const_String s2) {
    assert(s1.len == 0 || s1.str);
    assert(s2.len == 0 || s2.str);

    char c1 = '\0';
    char c2 = '\0';

    while (c1 == c2 && s1.len > 0 && s2.len > 0) {
        c1 = *s1.str++;
        s1.len--;

        c2 = *s2.str++;
        s2.len--;
    }

    if ((s1.len == 0) != (s2.len == 0))
        return (int)(s1.len - s2.len);

    return c1 - c2;
}

/**
 * @brief strcasecmp for const_String (not null-terminated strings)
 *
 * @param s1
 * @param s2
 * @return int
 */
inline int String_strcasecmp(const_String s1, const_String s2) {
    assert(s1.len == 0 || s1.str);
    assert(s2.len == 0 || s2.str);

    int c1 = '\0';
    int c2 = '\0';

    while (c1 == c2 && s1.len > 0 && s2.len > 0) {
        c1 = tolower(*s1.str++);
        s1.len--;

        c2 = tolower(*s2.str++);
        s2.len--;
    }

    if ((s1.len == 0) != (s2.len == 0))
        return (int)(s1.len - s2.len);

    return c1 - c2;
}

/**
 * @brief sscanf for String (not null-terminated strings)
 *
 * @param token
 * @param format
 * @param ...
 * @return int
 */
int String_sscanf(String token, const char* format, ...) __attribute__ ((format(scanf, 2, 3)));

/**
 * @brief isdigit() with + and -
 *
 * @param c
 * @return true
 * @return false
 */
inline bool maybedigit(int c) {
    return isdigit(c) || c == '-' || c == '+';
}

#endif // #ifndef MY_STRING_H_

#ifndef TEXT_LIB_H_
#define TEXT_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/**
 * @brief Struct to keep string and len together
 */
struct String {
    char* str = nullptr; ///< string
    ssize_t len = 0;     ///< string len
};

/**
 * @brief Tokenizator for lines
 *
 * @param str
 * @return String
 */
String my_strtok_line(const char* str);

/**
 * @brief Uses my_strtok_lines and writes results into array
 *
 * @param text
 * @param lines_array
 */
void split_text_to_lines(const char* text, String* lines_array);

/**
 * @brief Counts ch in str
 *
 * @param str
 * @param ch
 * @return size_t
 */
inline size_t cntchar(const char* str, const char ch) {
    size_t cnt = 0;

    for (size_t i = 0; str[i] != '\0'; i++)
        if (str[i] == ch)
            cnt++;

    return cnt;
}

#endif // #ifndef MY_STRING_H_
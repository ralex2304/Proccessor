#ifndef TEXT_PROCCESSOR_H_
#define TEXT_PROCCESSOR_H_

#include <stdio.h>

#include "lib/utils/macros.h"
#include "text_lib.h"
#include "lib/utils/statuses.h"
#include "lib/file/file.h"

/**
 * @brief Specifies input text file data
 */
struct InputData
{
    FILE* file = nullptr;

    char* text = nullptr;

    String* lines = nullptr;
    size_t lines_cnt = 0;

    Status::Statuses ctor(const char* filename);
    void detor();
};

#endif // #ifndef TEXT_PROCCESSOR_H_

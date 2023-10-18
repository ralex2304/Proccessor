#ifndef BUF_LIB_H_
#define BUF_LIB_H_

#include <stdlib.h>
#include <assert.h>

#include "macros.h"

struct Buffer {
    static const size_t INIT_CAPACITY = 256;

    char* data = nullptr;

    ssize_t capacity = 0;
    ssize_t size = 0;

    bool ctor(ssize_t init_capacity = INIT_CAPACITY);
    void dtor();

    bool resize(ssize_t new_capacity);

    bool cat(const char* cat, const size_t cat_size);
};


#endif //< #ifndef BUF_LIB_H_

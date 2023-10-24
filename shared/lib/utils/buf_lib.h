#ifndef BUF_LIB_H_
#define BUF_LIB_H_

#include <stdlib.h>
#include <assert.h>

#include "macros.h"

/**
 * @brief Rising buffer struct
 */
struct Buffer {
    static const size_t INIT_CAPACITY = 256;

    char* data = nullptr;   //< buffer data

    ssize_t capacity = 0;   //< current buf capacity
    ssize_t size = 0;       //< current data in buf size

    bool ctor(ssize_t init_capacity = INIT_CAPACITY);
    void dtor();

    /**
     * @brief Resizes buffer
     *
     * @param new_capacity
     * @return true
     * @return false
     */
    bool resize(ssize_t new_capacity);

    /**
     * @brief Concatenetes data to buffer
     *
     * @param cat
     * @param cat_size
     * @return true
     * @return false
     */
    bool cat(const char* cat, const size_t cat_size);
};


#endif //< #ifndef BUF_LIB_H_

#include "buf_lib.h"

bool Buffer::ctor(ssize_t init_capacity) {
    assert(init_capacity > 0);
    assert(data == nullptr);

    data = (char*)calloc(init_capacity, 1);

    if (data == nullptr)
        return false;

    capacity = init_capacity;
    return true;
}

void Buffer::dtor() {
    FREE(data);
}

bool Buffer::resize(ssize_t new_capacity) {
    assert(new_capacity > 0);

    char* tmp = data;
    data = (char*)recalloc(data, capacity, new_capacity);
    if (data == nullptr) {
        data = tmp;
        return false;
    }

    capacity = new_capacity;
    return true;
}

bool Buffer::cat(const char* cat, const size_t cat_size) {
    assert(cat);

    while (capacity < size + (ssize_t)cat_size)
        if (!resize(capacity * 2))
            return false;

    memcpy(data + size, cat, cat_size);
    size += cat_size;
    return true;
}

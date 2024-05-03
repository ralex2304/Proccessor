#include "asm.h"

bool Asm::ctor(const ssize_t buf_capacity) {
    if (buf_capacity > 0) // if buf constructor is not needed
        if (!buf.ctor(buf_capacity))
            return false;

    labels_capacity = DEFAULT_LABEL_ARR_SIZE;
    labels = (JumpLabel*)calloc(labels_capacity, sizeof(JumpLabel));
    if (labels == nullptr)
        return false;

    return true;
}

void Asm::dtor() {
    buf.dtor();
    FREE(labels);
    labels_capacity = 0;
}

bool Asm::resize_labels_arr() {
    assert(labels_capacity > 0);

    JumpLabel* tmp = (JumpLabel*)realloc(labels, labels_capacity * 2 * sizeof(JumpLabel));
    if (tmp == nullptr)
        return false;

    labels = tmp;

    for (size_t i = labels_capacity; i < labels_capacity * 2; i++)
        labels[i] = {};

    labels_capacity *= 2;

    return true;
};

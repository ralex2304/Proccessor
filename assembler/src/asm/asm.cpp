#include "asm.h"

bool Asm::ctor(const ssize_t buf_capacity) {
    if (buf_capacity > 0) // if buf constructor is not needed
        if (!buf.ctor(buf_capacity))
            return false;

    return true;
}

void Asm::dtor() {
    buf.dtor();
}

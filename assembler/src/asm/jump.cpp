#include "jump.h"

#define THROW_SYNTAX_ERROR_(text, ...)  do {                                                        \
            fprintf(stderr, CONSOLE_RED("Syntax error. " text) " Line %zu\n", __VA_ARGS__ line_num);\
            return Status::SYNTAX_ERROR;                                                            \
        } while(0)

Status::Statuses asm_new_label(JumpLabel* labels, const char* name,
                               const size_t byte, const size_t line_num) {
    assert(labels);
    assert(name);

    for (size_t i = 0; i < MAX_LABEL_NUM; i++) {
        if (labels[i].name != nullptr && strcmp(labels[i].name, name) == 0)
            THROW_SYNTAX_ERROR_("Label name \"%s\" used twice.", name,);
    }

    for (size_t i = 0; i < MAX_LABEL_NUM; i++) {
        if (labels[i].name == nullptr && labels[i].byte == -1) {
            labels[i].name = name;
            labels[i].byte = byte;
            return Status::NORMAL_WORK;
        }
    }

    THROW_SYNTAX_ERROR_("Too many labels specified. Max %zu.", MAX_LABEL_NUM,);
}

Status::Statuses asm_get_label_byte(JumpLabel* labels, const char* name,
                                    Imm_int_t* byte, const size_t line_num,
                                    const bool first_pass) {
    assert(labels);
    assert(name);

    for (size_t i = 0; i < MAX_LABEL_NUM; i++) {
        if (labels[i].name != nullptr && strcmp(labels[i].name, name) == 0) {
            *byte = labels[i].byte;
            return Status::NORMAL_WORK;
        }
    }

    if (!first_pass)
        THROW_SYNTAX_ERROR_("Unknown label \"%s\"", name,);

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

const char* asm_get_label_name(const JumpLabel* labels, const Imm_int_t byte) {
    for (size_t i = 0; i < MAX_LABEL_NUM; i++) {
        if (labels[i].byte == byte)
            return labels[i].name;
    }

    return "!Label name not found!";
}

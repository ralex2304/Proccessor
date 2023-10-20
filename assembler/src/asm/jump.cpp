#include "jump.h"

#define THROW_SYNTAX_ERROR_(...)  return asm_throw_syntax_error(name,       \
                                                                inp_file,   \
                                                                __VA_ARGS__)

Status::Statuses asm_new_label(JumpLabel* labels, const String name,
                               const InputFileInfo* inp_file, const size_t byte) {
    assert(labels);
    assert(name.str);
    assert(inp_file);

    for (size_t i = 0; i < MAX_LABEL_NUM; i++) {
        if (String_strcmp(String_TO_c(labels[i].name), String_TO_c(name)) == 0)
            THROW_SYNTAX_ERROR_("Label name \"%.*s\" used twice.", String_PRINTF(name));
    }

    for (size_t i = 0; i < MAX_LABEL_NUM; i++) {
        if (labels[i].name.str == nullptr && labels[i].name.len == 0 && labels[i].byte == -1) {
            labels[i].name = name;
            labels[i].byte = byte;
            return Status::NORMAL_WORK;
        }
    }

    THROW_SYNTAX_ERROR_("Too many labels specified. Max %zu.", MAX_LABEL_NUM);
}

Status::Statuses asm_get_label_byte(JumpLabel* labels, const String name,
                                    const InputFileInfo* inp_file, Imm_int_t* byte,
                                    const bool final_pass) {
    assert(labels);
    assert(name.str);
    assert(inp_file);
    assert(byte);

    for (size_t i = 0; i < MAX_LABEL_NUM; i++) {
        if (String_strcmp(String_TO_c(labels[i].name), String_TO_c(name)) == 0) {
            *byte = labels[i].byte;
            return Status::NORMAL_WORK;
        }
    }

    if (final_pass)
        THROW_SYNTAX_ERROR_("Unknown label \"%.*s\"", String_PRINTF(name));

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

c_String asm_get_label_name(const JumpLabel* labels, const Imm_int_t byte) {
    for (size_t i = 0; i < MAX_LABEL_NUM; i++) {
        if (labels[i].byte == byte)
            return {labels[i].name.str, labels[i].name.len};
    }

    return String_CONST("!Label name not found!");
}

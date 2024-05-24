#include "jump.h"

#define THROW_SYNTAX_ERROR_(...)  return asm_throw_syntax_error(asm_data, line, __VA_ARGS__)

Status::Statuses asm_new_label(const String name, const size_t addr,
                               Asm* asm_data, const AsmLine* line) {
    assert(name.str);
    assert(asm_data);

    for (size_t i = 0; i < asm_data->labels_capacity; i++) {
        if (String_strcmp(String_TO_const(asm_data->labels[i].name), String_TO_const(name)) == 0)
            THROW_SYNTAX_ERROR_("Label name \"%.*s\" used twice.", String_PRINTF(name));
    }

    for (size_t i = 0; i < asm_data->labels_capacity; i++) {
        if (asm_data->labels[i].name.len == 0 && asm_data->labels[i].addr == -1) {

            asm_data->labels[i].name = name;
            asm_data->labels[i].addr = (ssize_t)addr;
            return Status::NORMAL_WORK;
        }
    }

    size_t old_cap = asm_data->labels_capacity;

    if (!asm_data->resize_labels_arr())
        return Status::MEMORY_EXCEED;

    asm_data->labels[old_cap].name = name;
    asm_data->labels[old_cap].addr = (ssize_t)addr;

    return Status::NORMAL_WORK;
}

Status::Statuses asm_get_label_addr(const String name, const Asm* asm_data,
                                    const AsmLine* line, Imm_int_t* addr) {
    assert(name.str);
    assert(asm_data);
    assert(line);
    assert(addr);

    for (size_t i = 0; i < asm_data->labels_capacity; i++) {
        if (String_strcmp(String_TO_const(asm_data->labels[i].name), String_TO_const(name)) == 0) {
            *addr = asm_data->labels[i].addr;
            return Status::NORMAL_WORK;
        }
    }

    if (asm_data->pass_num == asm_data->LAST_PASS_NUM)
        THROW_SYNTAX_ERROR_("Unknown label \"%.*s\"", String_PRINTF(name));

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

const_String asm_get_label_name(const Asm* asm_data, const Imm_int_t addr) {
    for (size_t i = 0; i < asm_data->labels_capacity; i++) {
        if (asm_data->labels[i].addr == addr)
            return {asm_data->labels[i].name.str, asm_data->labels[i].name.len};
    }

    return String_CONST("!Label name not found!");
}

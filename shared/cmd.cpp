#include "cmd.h"

const CmdInfo* find_command_by_num(const Cmd_num_t num) {
    if (num == CMDS_DICT[num].num)
        return CMDS_DICT + num;

    assert(0 && "Command num and index in CMDS_DICT array must be the same");
}

const CmdInfo* find_command_by_name(const char* name) {
    assert(name);

    for (size_t i = 0; i < CMDS_DICT_SIZE; i++) {
        if (strcmp(CMDS_DICT[i].name, name) == 0)
            return CMDS_DICT + i;
    }

    return nullptr;
}

const RegInfo* find_reg_by_num(const RegNum_t reg) {
    if (reg == REGS_DICT[reg].num)
        return REGS_DICT + reg;

    assert(0 && "Ref num and index in REGS_DICT array must be the same");
}

const RegInfo* find_reg_by_name(const char* name) {
    assert(name);

    for (size_t i = 0; i < REGS_NUM; i++) {
        if (strcmp(REGS_DICT[i].name, name) == 0)
            return REGS_DICT + i;
    }

    return nullptr;
}

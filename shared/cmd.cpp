#include "cmd.h"

const Cmd* find_command_by_num(const Cmd_num_t num) {
    if (num == CMDS[num].num)
        return CMDS + num;

    assert(0 && "Command num and index in CMDS array must be the same");
}

const Cmd* find_command_by_name(const char* name) {
    assert(name);

    for (size_t i = 0; i < CMDS_NUM; i++) {
        if (strcmp(CMDS[i].name, name) == 0)
            return CMDS + i;
    }

    return nullptr;
}

const Reg* find_reg_by_num(const RegNum_t reg) {
    if (reg == REGS[reg - 1].num)
        return REGS + reg - 1;

    assert(0 && "Ref num and index in REGS array must be the same");
}

const Reg* find_reg_by_name(const char* name) {
    assert(name);

    for (size_t i = 0; i < REGS_NUM; i++) {
        if (strcmp(REGS[i].name, name) == 0)
            return REGS + i;
    }

    return nullptr;
}

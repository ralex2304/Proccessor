#include "cmd.h"

bool Signature::check() const {
    if (strncmp(name, SIGNATURE.name, sizeof(name)) != 0) {
        static_assert(sizeof(name) == 2);

        printf(CONSOLE_RED("Error. Wrong signature.") " \"%c%c\" instead of \"%c%c\"\n",
               name[0], name[1], SIGNATURE.name[0], SIGNATURE.name[1]);
        return false;
    }

    if (version != SIGNATURE.version) {
        printf(CONSOLE_RED("Error. Wrong version.") " \"%d\" instead of \"%d\"\n",
               version, SIGNATURE.version);
        return false;
    }

    return true;
}

size_t Cmd::size() const {
    return byte.reg * sizeof(args.reg)
         + byte.imm * (byte.ram ? sizeof(args.imm_ram)
                                : sizeof(args.imm));
}

const CmdInfo* find_command_by_num(const Cmd_num_t num) {
    if (num >= CMDS_DICT_SIZE)
        return nullptr;

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
    if (reg >= REGS_NUM)
        return nullptr;

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

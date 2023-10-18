#include "cmd.h"

bool FileHeader::check() const {
    if (memcmp(sign, FILE_HEADER.sign, sizeof(sign)) != 0) {
        static_assert(sizeof(sign) == 2);

        printf(CONSOLE_RED("Error. Wrong signature.") " \"%c%c\" instead of \"%c%c\"\n",
               sign[0], sign[1], FILE_HEADER.sign[0], FILE_HEADER.sign[1]);
        return false;
    }

    if (version != FILE_HEADER.version) {
        printf(CONSOLE_RED("Error. Wrong version.") " \"%d\" instead of \"%d\"\n",
               version, FILE_HEADER.version);
        return false;
    }

    return true;
}

size_t Cmd::size() const {
    return byte.reg * sizeof(args.reg)
         + byte.imm * (byte.ram ? sizeof(args.imm_int)
                                    : sizeof(args.imm_double));
}

const CmdInfo* find_command_by_num(const Cmd_num_t num) {
    for (size_t i = 0; i < CMDS_DICT_SIZE; i++)
        if (CMDS_DICT[i].num == num)
            return CMDS_DICT + i;

    return nullptr;
}

const CmdInfo* find_command_by_name(const char* name) {
    assert(name);

    for (size_t i = 0; i < CMDS_DICT_SIZE; i++)
        if (strcasecmp(CMDS_DICT[i].name, name) == 0)
            return CMDS_DICT + i;

    return nullptr;
}

const RegInfo* find_reg_by_num(const RegNum_t num) {
    for (size_t i = 0; i < REGS_NUM; i++)
        if (REGS_DICT[i].num == num)
            return REGS_DICT + i;

    return nullptr;
}

const RegInfo* find_reg_by_name(const char* name) {
    assert(name);

    for (size_t i = 0; i < REGS_NUM; i++)
        if (strcmp(REGS_DICT[i].name, name) == 0)
            return REGS_DICT + i;

    return nullptr;
}

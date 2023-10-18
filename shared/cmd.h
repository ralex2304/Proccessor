#ifndef CMD_H_
#define CMD_H_

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <inttypes.h>

#include "lib/utils/console.h"

const short CMD_VERSION = 0;

const size_t MAX_LABEL_NUM = 128;

const size_t MAX_LINE_LEN = 64;   //< max command text line len

struct CmdByte {
    unsigned char num: 5;
    bool reg: 1;
    bool imm: 1;
    bool ram: 1;
};

static_assert(sizeof(CmdByte) == 1);

const unsigned char CMD_BYTE_NUM_BIT_MASK = 0b0001'1111;

typedef unsigned char RegNum_t;

enum RegNums: RegNum_t {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
};

struct RegInfo {
    const RegNums num;
    const char* name = nullptr;
};

const RegInfo REGS_DICT[] {
    {RAX, "rax"},
    {RBX, "rbx"},
    {RCX, "rcx"},
    {RDX, "rdx"},
};

const RegNum_t REGS_NUM = sizeof(REGS_DICT) / sizeof(RegInfo); //< SPU registers number

typedef double Imm_double_t;

#define IMM_DOUBLE_T_EPSILON    0.000001

#define IMM_DOUBLE_T_PRINTF     "%lf"

#define CHECK_IMM_DOUBLE(var) !isnan(var)


typedef long Imm_int_t;

#define IMM_INT_T_PRINTF "%ld"

struct CmdArgs {
    Imm_double_t imm_double = NAN;
    unsigned char reg = -1;
    Imm_int_t imm_int = 0;
};

struct ArgsEn {
    bool reg:           1;
    bool imm_double:    1;
    bool imm_int:       1;
    bool ram:           1;
    bool label:         1;
};

typedef unsigned char Cmd_num_t;



enum CmdNum: Cmd_num_t {
    #define DEF_CMD(name, num, ...) CMD_##name = num,

    #include "cmd_dict.h"

    #undef DEF_CMD
};

struct CmdInfo {
    const CmdNum num;
    const char* name = nullptr;
    ArgsEn args = {};

    const char* description = nullptr;
};


#define DEF_CMD(name, num, args, description, ...) {CMD_##name, #name,              \
                                                        {(bool) (args & 0b10000),   \
                                                         (bool) (args & 0b01000),   \
                                                         (bool) (args & 0b00100),   \
                                                         (bool) (args & 0b00010),   \
                                                         (bool) (args & 0b00001)},  \
                                                        description},

const CmdInfo CMDS_DICT[] {
    #include "cmd_dict.h"
};
#undef DEF_CMD

const size_t CMDS_DICT_SIZE = sizeof(CMDS_DICT) / sizeof(CmdInfo); //< Number of commands

struct FileHeader {
    char sign[2] = {'K','U'};
    short version = CMD_VERSION;

    bool check() const;
};

static_assert(sizeof(FileHeader) == 4);

const FileHeader FILE_HEADER = {};

struct Cmd {
    const CmdInfo* info = nullptr;

    CmdByte byte = {};
    CmdArgs args = {};

    size_t size() const;
};

/**
 * @brief Finds command in CMDS_DICT by num
 *
 * @param num
 * @return const Command*
 */
const CmdInfo* find_command_by_num(const Cmd_num_t num);

/**
 * @brief Finds command in CMDS_DICT[] by name
 *
 * @param name
 * @return const Command*
 */
const CmdInfo* find_command_by_name(const char* name);

/**
 * @brief Finds reg in REGS_DICT[] by num
 *
 * @param reg
 * @return const RegInfo*
 */
const RegInfo* find_reg_by_num(const RegNum_t reg);

/**
 * @brief Finds reg in REGS_DICT[] by name
 *
 * @param name
 * @return const RegInfo*
 */
const RegInfo* find_reg_by_name(const char* name);

#endif // #ifndef CMD_H_

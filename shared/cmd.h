#ifndef CMD_H_
#define CMD_H_

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "lib/utils/console.h"

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

typedef double Imm_t;

#define IMM_T_PRINTF "%lf"

typedef long Imm_ram_t;

#define IMM_RAM_T_PRINTF "%ld"

struct CmdArgs {
    Imm_t imm = NAN;
    unsigned char reg = -1;
    Imm_ram_t imm_ram = 0;
};

struct ArgsEn {
    bool reg: 1;
    bool imm: 1;
    bool ram: 1;
};

typedef unsigned char Cmd_num_t;

enum CmdNum: Cmd_num_t {
    CMD_HLT  =  0,
    CMD_PUSH =  1,
    CMD_POP  =  2,
    CMD_IN   =  3,
    CMD_OUT  =  4,
    CMD_ADD  =  5,
    CMD_SUB  =  6,
    CMD_MUL  =  7,
    CMD_DIV  =  8,
    CMD_SQRT =  9,
    CMD_SIN  = 10,
    CMD_COS  = 11,
};

struct CmdInfo {
    const CmdNum num;
    const char* name = nullptr;
    ArgsEn args;

    const char* description = nullptr;
};

const short CMD_VERSION = 0;

const CmdInfo CMDS_DICT[] {

    {CMD_HLT,   "HLT",  {},                     "halt - end of program"},
    {CMD_PUSH,  "push", {true,  true,  true},   "push one element to stack ||"
                                                " take from reg and push ||"
                                                " take from reg + imm and push"},
    {CMD_POP,   "pop",  {true,  false, true},   "pop from stack and write to reg"},
    {CMD_IN,    "in",   {},                     "push one element to stack from user input"},
    {CMD_OUT,   "out",  {},                     "pop and print element from stack"},
    {CMD_ADD,   "add",  {},                     "+"},
    {CMD_SUB,   "sub",  {},                     "-"},
    {CMD_MUL,   "mul",  {},                     "*"},
    {CMD_DIV,   "div",  {},                     "/"},
    {CMD_SQRT,  "sqrt", {},                     "sqaure root"},
    {CMD_SIN,   "sin",  {},                     "sinus"},
    {CMD_COS,   "cos",  {},                     "cosinus"},

};

const size_t CMDS_DICT_SIZE = sizeof(CMDS_DICT) / sizeof(CmdInfo); //< Number of commands

struct Signature {
    char name[2] = {'K','U'};
    short version = CMD_VERSION;

    bool check() const;
};

static_assert(sizeof(Signature) == 4);

const Signature SIGNATURE = {};

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

#ifndef CMD_H_
#define CMD_H_

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>



struct CmdByte {
    unsigned char num: 5;
    bool reg: 1;
    bool imm: 1;
};

const size_t MAX_LINE_LEN = 64;   //< max command text line len

typedef unsigned char RegNum_t;

const RegNum_t REGS_NUM = 4; //< SPU registers number

enum RegNums: RegNum_t {
    RERROR = 0,
    RAX    = 1,
    RBX    = 2,
    RCX    = 3,
    RDX    = 4,
};

struct Reg {
    const RegNums num;
    const char* name = nullptr;
};

const Reg REGS[] {
    {RAX, "rax"},
    {RBX, "rbx"},
    {RCX, "rcx"},
    {RDX, "rdx"},
};

typedef double Imm_t;

#define IMM_T_PRINTF "%lf"


struct CmdArgs {
    Imm_t imm = NAN;
    unsigned char reg = -1;
};

struct ArgsEn {
    bool reg: 1;
    bool imm: 1;
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

struct Cmd {
    const CmdNum num;
    const char* name = nullptr;
    ArgsEn args;

    const char* description = nullptr;
};

const Cmd CMDS[] {

    {CMD_HLT,   "HLT",  {},             "halt - end of program"},
    {CMD_PUSH,  "push", {true,  true }, "push one element to stack || take from reg and push || take from reg + imm and push"},
    {CMD_POP,   "pop",  {true,  false}, "pop from stack and write to reg"},
    {CMD_IN,    "in",   {},             "push one element to stack from user input"},
    {CMD_OUT,   "out",  {},             "pop and print element from stack"},
    {CMD_ADD,   "add",  {},             "+"},
    {CMD_SUB,   "sub",  {},             "-"},
    {CMD_MUL,   "mul",  {},             "*"},
    {CMD_DIV,   "div",  {},             "/"},
    {CMD_SQRT,  "sqrt", {},             "sqaure root"},
    {CMD_SIN,   "sin",  {},             "sinus"},
    {CMD_COS,   "cos",  {},             "cosinus"},

};

const size_t CMDS_NUM = sizeof(CMDS) / sizeof(Cmd); //< Number of commands


/**
 * @brief Finds command in CMDS by num
 *
 * @param num
 * @return const Command*
 */
const Cmd* find_command_by_num(const Cmd_num_t num);

/**
 * @brief Finds command in CMDS[] by name
 *
 * @param name
 * @return const Command*
 */
const Cmd* find_command_by_name(const char* name);

/**
 * @brief Finds reg in REGS[] by num
 *
 * @param reg
 * @return const Reg*
 */
const Reg* find_reg_by_num(const RegNum_t reg);

/**
 * @brief Finds reg in REGS[] by name
 *
 * @param name
 * @return const Reg*
 */
const Reg* find_reg_by_name(const char* name);

#endif // #ifndef CMD_H_

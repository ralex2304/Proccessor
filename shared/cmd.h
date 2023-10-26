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
#include "lib/text/text_lib.h"

const short CMD_VERSION = 1;        //< commands dict version

const size_t MAX_LABEL_NUM = 128;   //< maximum number of labels in program (array size)

const size_t MAX_LINE_LEN = 64;     //< max command text line len

const size_t CMD_MAX_NUM = 255;

/**
 * @brief Specifies command bit field
 */
struct CmdKeys {
    unsigned char num:  8;  //< Command number
    bool reg:           1;  //< reg argument flag
    bool imm_double:    1;  //< double immutable argument flag
    bool imm_int:       1;  //< int immutable argument flag
    bool ram:           1;  //< ram argument flag
};

const unsigned char CMD_BYTE_NUM_BIT_MASK = 0xFF; //< Bit field assignment kostyl'

typedef unsigned char RegNum_t; //< Reg number type

/**
 * @brief Specifies regs names
 */
enum RegNums: RegNum_t {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
};

/**
 * @brief Specifies information about register
 */
struct RegInfo {
    const RegNums num = {};     //< reg number
    const const_String name = {};   //< reg name
};

/**
 * @brief regs dictionary
 */
const RegInfo REGS_DICT[] {
    {RAX, String_CONST("rax")},
    {RBX, String_CONST("rbx")},
    {RCX, String_CONST("rcx")},
    {RDX, String_CONST("rdx")},
};

const RegNum_t REGS_NUM = sizeof(REGS_DICT) / sizeof(RegInfo); //< SPU registers number

typedef double Imm_double_t;    //< double immutable arguments type

#define IMM_DOUBLE_T_EPSILON    1e-6

#define IMM_DOUBLE_T_PRINTF     "%lf"

#define CHECK_IMM_DOUBLE(var) !isnan(var)


typedef long Imm_int_t; //< int immutable arguments type

#define IMM_INT_T_PRINTF      "%ld"

/**
 * @brief Used for carriyng read values
 */
struct CmdArgs {
    Imm_double_t imm_double = NAN;
    unsigned char reg = -1;
    Imm_int_t imm_int = 0;
};

/**
 * @brief Specifies arguments types, that command is able to recieve
 */
struct ArgsEnabled {
    bool reg:           1;  //< register arg
    bool imm_double:    1;  //< double immutable arg
    bool imm_int:       1;  //< int immutable arg
    bool ram:           1;  //< ram argument
    bool label:         1;  //< label
};

/**
 * @brief Available options enabling bits
 */
enum ArgsEnabledOptions: size_t {
    ARG_NONE        = 0,
    ARG_REG         = 1 << 0,
    ARG_IMM_DOUBLE  = 1 << 1,
    ARG_IMM_INT     = 1 << 2,
    ARG_RAM         = 1 << 3,
    ARG_LABEL       = 1 << 4,
};

typedef unsigned char Cmd_num_t;    //< command number type

/**
 * @brief Specifies commands names and numbers
 */
enum CmdNum: Cmd_num_t {
    #define DEF_CMD(name, num, ...) CMD_##name = num,

    #include "cmd_dict.h"

    #undef DEF_CMD
};

/**
 * @brief Keeps information about commans
 */
struct CmdInfo {
    const CmdNum num;           //< command number
    const const_String name = {};   //< command name
    ArgsEnabled is_args = {};   //< possible arguments

    const char* description = nullptr;  //< command description (for help)
};

/**
 * @brief Commands dicitonary
 */
const CmdInfo CMDS_DICT[] {
    #define DEF_CMD(name, num, args, description, ...) {CMD_##name, {#name, sizeof(#name) - 1}, \
                                                            {(bool) ((args) & 0b00001),         \
                                                             (bool) ((args) & 0b00010),         \
                                                             (bool) ((args) & 0b00100),         \
                                                             (bool) ((args) & 0b01000),         \
                                                             (bool) ((args) & 0b10000)},        \
                                                             description},
    #include "cmd_dict.h"

    #undef DEF_CMD
};

const size_t CMDS_DICT_SIZE = sizeof(CMDS_DICT) / sizeof(CmdInfo); //< Number of commands

/**
 * @brief Binary file header
 */
struct FileHeader {
    uint16_t sign =  0x554B;        //< signature ("KU")
    short version = CMD_VERSION;    //< commands version

    bool check() const;
};

static_assert(sizeof(FileHeader) == 4); //< header must be 4 bytes

const FileHeader FILE_HEADER = {};  //< file header const. Used as the standart

/**
 * @brief Command data structure
 */
struct Cmd {
    const CmdInfo* info = nullptr;  //< pointer to CMD_DICT

    CmdKeys keys   = {};    //< command keys
    CmdArgs args   = {};    //< command args

    size_t size() const;            //< returns command size in bytes
};

/**
 * @brief Finds command in CMDS_DICT by num
 *
 * @param num
 * @return const CmdInfo*
 */
const CmdInfo* find_command_by_num(const Cmd_num_t num);

/**
 * @brief Finds command in CMDS_DICT[] by name
 *
 * @param name
 * @return const CmdInfo*
 */
const CmdInfo* find_command_by_name(const String name);

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
const RegInfo* find_reg_by_name(const String name);

#endif // #ifndef CMD_H_

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define DEBUG           //< Enables debug mode

/// Stack debug options
#define CANARY_PROTECT  //< Enables canary protection
#define HASH_PROTECT    //< Enables hash protection

const char* const STACK_LOG_FILENAME = "stack.log";

const size_t COMMAND_MAX_ARGS_COUNT = 2; //< Maximum command args number

const size_t MAX_LINE_LEN = 64; //< max command text line len

typedef unsigned char Command_num_t;

enum class ArgType {
    NONE   = 0,
    DOUBLE = 1,
};

union ArgVar {
    double double_arg = NAN;
};

enum CommandsNum: Command_num_t {
    COM_HLT   =  0,
    COM_PUSH  =  1,
    COM_IN    =  2,
    COM_OUT   =  3,
    COM_ADD   =  4,
    COM_SUB   =  5,
    COM_MUL   =  6,
    COM_DIV   =  7,
    COM_SQRT  =  8,
    COM_SIN   =  9,
    COM_COS   = 10,
};

struct Command {
    const CommandsNum num;
    const char* name;
    ArgType args[COMMAND_MAX_ARGS_COUNT];
    const char* description;
};

const Command COMMANDS[] {

    {COM_HLT,   "HLT",  {}               , "halt - end of program"},
    {COM_PUSH,  "push", {ArgType::DOUBLE}, "push one element to stack"},
    {COM_IN,    "in",   {}               , "push one element to stack from user input"},
    {COM_OUT,   "out",  {}               , "pop and print element from stack"},
    {COM_ADD,   "add",  {}               , "+"},
    {COM_SUB,   "sub",  {}               , "-"},
    {COM_MUL,   "mul",  {}               , "*"},
    {COM_DIV,   "div",  {}               , "/"},
    {COM_SQRT,  "sqrt", {}               , "sqaure root"},
    {COM_SIN,   "sin",  {}               , "sinus"},
    {COM_COS,   "cos",  {}               , "cosinus"},

};

const size_t COMMANDS_NUM = sizeof(COMMANDS) / sizeof(Command); //< Number of commands

/**
 * @brief Returns string with command argument type name
 *
 * @param type
 * @return const char*
 */
inline const char* com_arg_type_str(ArgType type) {
    switch (type) {
        case ArgType::DOUBLE:
            return "double";
        case ArgType::NONE:
        default:
            assert(0 && "Wong ArgType passed");
    }
}

/**
 * @brief Returns sizeof argument var
 *
 * @param type
 * @return size_t
 */
inline size_t com_arg_type_sizeof(ArgType type) {
    switch (type){
        case ArgType::DOUBLE:
            return sizeof(double);
        case ArgType::NONE:
        default:
            assert(0 && "Wong ArgType passed");
    }
}

#endif // #ifndef COMMANDS_H_

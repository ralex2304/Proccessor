#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdlib.h>

const size_t COMMAND_MAX_ARGS_COUNT = 2;

const size_t MAX_LINE_LEN = 64; //< max command text line len

enum class ArgType {
    NONE   = 0,
    DOUBLE = 1,
};

struct Command {
    unsigned char num;
    const char* name;
    ArgType args[COMMAND_MAX_ARGS_COUNT];
    const char* description;
};

const Command COMMANDS[] {

    { 0, "HLT",  {}               , "halt - end of program"},
    { 1, "push", {ArgType::DOUBLE}, "push one element to stack"},
    { 2, "in",   {}               , "push one element to stack from user input"},
    { 3, "out",  {}               , "pop and print element from stack"},
    { 4, "add",  {}               , "+"},
    { 5, "sub",  {}               , "-"},
    { 6, "mul",  {}               , "*"},
    { 7, "div",  {}               , "/"},
    { 8, "mod",  {}               , "%"},
    { 9, "sqrt", {}               , "sqaure root"},
    {10, "sin",  {}               , "sinus"},
    {11, "cos",  {}               , "cosinus"},

};

const size_t COMMANDS_NUM = sizeof(COMMANDS) / sizeof(Command); //< Number of commands

const size_t COMMAND_NUM_SIZE = sizeof(Command::num);

#endif // #ifndef COMMANDS_H_

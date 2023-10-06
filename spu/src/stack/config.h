#ifndef STK_CONFIG_H_
#define STK_CONFIG_H_

#include <math.h>

#include "cmd.h"
#include "stack.h"

#define DEBUG           //< Enables debug mode

/// Stack debug options
#define CANARY_PROTECT  //< Enables canary protection
#define HASH_PROTECT    //< Enables hash protection

const char* const STACK_LOG_FILENAME = "stack.log";

typedef double Elem_t;                      //< stack elements type
static const Elem_t ELEM_T_POISON = NAN;    //< poison value for stack
#define ELEM_T_PRINTF   "%lf"                //< Elem_t printf specificator

inline bool is_poison(Elem_t elem) {        //< Poison checking function
    return isnan(elem);
}

typedef unsigned long long Canary_t;                             //< canary type
static const Canary_t CANARY_VAL = 0x8BADF00DDEADDEAD;           //< canary protect value
#define CANARY_T_PRINTF "%llX"                                   //< Canary_t printf specificator


#endif /// #ifndef STK_CONFIG_H_

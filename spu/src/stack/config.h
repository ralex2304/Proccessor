#ifndef CONFIG_H_
#define CONFIG_H_

#include <math.h>

#include "../../../Commands.h"
#include "stack.h"


typedef double Elem_t;                      //< stack elements type
static const Elem_t ELEM_T_POISON = NAN;    //< poison value for stack
#define ELEM_T_PRINTF   "%d"                //< Elem_t printf specificator

inline bool is_poison(Elem_t elem) {        //< Poison checking function
    return isnan(elem);
}

typedef unsigned long long Canary_t;                             //< canary type
static const Canary_t CANARY_VAL = 0x8BADF00DDEADDEAD;           //< canary protect value
#define CANARY_T_PRINTF "%llX"                                   //< Canary_t printf specificator


#endif /// #ifndef CONFIG_H_

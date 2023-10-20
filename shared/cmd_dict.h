#ifndef DEF_CMD
static_assert(0 && "DEF_CMD is not defined");
#endif

//    name | num | args | description | spu algorithm

// args: reg | imm_double | imm_int | ram | label

DEF_CMD(hlt,   0, 0b00000, "halt - end of program", {HALT();})

DEF_CMD(push,  1, 0b11110, "push one element to stack || take from reg and push ||"
                                       " take from reg + imm_double and push", {

    CHECK_AND_THROW_ERR(IS_ARG_REG || IS_ARG_IMM, "\"push\" requires at least one argument.");

    IMM_DOUBLE_T a = 0;

    if (IS_ARG_REG)
        a += REG(ARG_REG);

    if (IS_ARG_IMM)
        a += ARG_IMM_DOUBLE;

    PUSH(a);
})

DEF_CMD(pop,   2, 0b10110,  "pop from stack and write to reg", {
    CHECK_AND_THROW_ERR(IS_ARG_REG, "\"pop\" requires reg.");

    POP(&REG(ARG_REG));
})

DEF_CMD(jmp,   3, 0b00001, "jump", {
    CHECK_AND_THROW_ERR(IS_ARG_IMM, "\"jump\" commands require byte address.");

    JUMP(ARG_IMM_INT);
})

DEF_CMD(ja,    4, 0b00001, "jump >", {
    CHECK_AND_THROW_ERR(IS_ARG_IMM, "\"jump\" commands require byte address.");

    JUMP_CLAUSE_FUNC(IS_GREATER_IMM_DOUBLE, ARG_IMM_INT);
})

DEF_CMD(jae,   5, 0b00001, "jump >=", {
    CHECK_AND_THROW_ERR(IS_ARG_IMM, "\"jump\" commands require byte address.");

    JUMP_CLAUSE_FUNC(IS_GREATER_EQUAL_IMM_DOUBLE, ARG_IMM_INT);
})

DEF_CMD(jb,    6, 0b00001, "jump <", {
    CHECK_AND_THROW_ERR(IS_ARG_IMM, "\"jump\" commands require byte address.");

    JUMP_CLAUSE_FUNC(IS_LOWER_IMM_DOUBLE, ARG_IMM_INT);
})

DEF_CMD(jbe,   7, 0b00001, "jump <=", {
    CHECK_AND_THROW_ERR(IS_ARG_IMM, "\"jump\" commands require byte address.");

    JUMP_CLAUSE_FUNC(IS_LOWER_EQUAL_IMM_DOUBLE, ARG_IMM_INT);
})

DEF_CMD(je,    8, 0b00001, "jump ==", {
    CHECK_AND_THROW_ERR(IS_ARG_IMM, "\"jump\" commands require byte address.");

    JUMP_CLAUSE_FUNC(IS_EQUAL_IMM_DOUBLE, ARG_IMM_INT);
})

DEF_CMD(jne,   9, 0b00001, "jump !=", {
    CHECK_AND_THROW_ERR(IS_ARG_IMM, "\"jump\" commands require byte address.");

    JUMP_CLAUSE_FUNC(!IS_EQUAL_IMM_DOUBLE, ARG_IMM_INT);
})

DEF_CMD(jf,   10, 0b00001, "jump on Fridays", {
    CHECK_AND_THROW_ERR(IS_ARG_IMM, "\"jump\" commands require byte address.");

    if (WEEKDAY() == 5)
        JUMP(ARG_IMM_INT);
})

DEF_CMD(in ,  16, 0b00000, "push one element to stack from user input", {
    IMM_DOUBLE_T a = 0;
    GET_FROM_INPUT(&a);

    PUSH(a);
})

DEF_CMD(out,  17, 0b00000, "pop and print element from stack", {
    IMM_DOUBLE_T a = 0;
    POP(&a);

    PRINT(a);
})

DEF_CMD(add,  18, 0b00000, "+",           {BINARY_OPERATOR(+);})
DEF_CMD(sub,  19, 0b00000, "-",           {BINARY_OPERATOR(-);})
DEF_CMD(mul,  20, 0b00000, "*",           {BINARY_OPERATOR(*);})
DEF_CMD(div,  21, 0b00000, "/",           {BINARY_OPERATOR(/);})
DEF_CMD(sqrt, 22, 0b00000, "sqaure root", {UNARY_OPERATOR(sqrt);})
DEF_CMD(sin,  23, 0b00000, "sinus",       {UNARY_OPERATOR(sin);})
DEF_CMD(cos,  24, 0b00000, "cosinus",     {UNARY_OPERATOR(cos);})



DEF_CMD(dmp,  31, 0b00000, "spu data dump", {
    DUMP();
})

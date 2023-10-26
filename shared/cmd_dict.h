#ifndef DEF_CMD
static_assert(0 && "DEF_CMD is not defined");
#endif

//    name | num | args | description | spu algorithm

DEF_CMD(hlt,   0, ARG_NONE, "halt - end of program", {HALT();})

DEF_CMD(push,  1, ARG_REG | ARG_IMM_DOUBLE | ARG_IMM_INT | ARG_RAM | ARG_LABEL,
       "push to stack", {

    CHECK_AND_THROW_ERR(IS_ARG_REG || IS_ARG_IMM_DOUBLE || IS_ARG_IMM_INT,
                        "\"push\" requires at least one argument.");

    PUSH(GET_RVALUE());
})

DEF_CMD(pop,   2, ARG_REG | ARG_IMM_INT | ARG_RAM,  "pop from stack and write somewhere", {
    CHECK_AND_THROW_ERR(IS_ARG_REG || IS_ARG_IMM_INT, "\"pop\" requires reg.");

    POP(GET_LVALUE_PTR());
})

DEF_CMD(jmp,   3, ARG_REG | ARG_LABEL, "jump", {
    JUMP_CHECK_ARGS();

    JUMP(JUMP_DESTINATION());
})

DEF_CMD(ja,    4, ARG_REG | ARG_LABEL, "jump >", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_GREATER_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jae,   5, ARG_REG | ARG_LABEL, "jump >=", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_GREATER_EQUAL_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jb,    6, ARG_REG | ARG_LABEL, "jump <", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_LOWER_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jbe,   7, ARG_REG | ARG_LABEL, "jump <=", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_LOWER_EQUAL_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(je,    8, ARG_REG | ARG_LABEL, "jump ==", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_EQUAL_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jne,   9, ARG_REG | ARG_LABEL, "jump !=", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(!IS_EQUAL_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jf,   10, ARG_REG | ARG_LABEL, "jump on Fridays", {
    JUMP_CHECK_ARGS();

    if (WEEKDAY() == 5)
        JUMP(JUMP_DESTINATION());
})

DEF_CMD(call, 11, ARG_REG | ARG_LABEL, "call - go to procedure", {
    JUMP_CHECK_ARGS();

    PUSH((IMM_DOUBLE_T)GET_NEXT_INSTRUCTION_PTR());

    JUMP(JUMP_DESTINATION());
})

DEF_CMD(ret,  12, ARG_NONE, "ret - end of procedure", {
    IMM_DOUBLE_T a = 0;

    POP(&a);

    IMM_INT_T b = (IMM_INT_T)a;

    JUMP(b);
})



DEF_CMD(in ,  16, ARG_NONE, "push one element to stack from user input", {
    IMM_DOUBLE_T a = 0;
    GET_FROM_INPUT(&a);

    PUSH(a);
})

DEF_CMD(out,  17, ARG_NONE, "pop and print element from stack", {
    IMM_DOUBLE_T a = 0;
    POP(&a);

    PRINT(a);
})

DEF_CMD(add,  18, ARG_NONE, "+",           {BINARY_OPERATOR(+);})
DEF_CMD(sub,  19, ARG_NONE, "-",           {BINARY_OPERATOR(-);})
DEF_CMD(mul,  20, ARG_NONE, "*",           {BINARY_OPERATOR(*);})
DEF_CMD(div,  21, ARG_NONE, "/",           {BINARY_OPERATOR(/);})
DEF_CMD(sqrt, 22, ARG_NONE, "sqaure root", {UNARY_OPERATOR(sqrt);})
DEF_CMD(sin,  23, ARG_NONE, "sinus",       {UNARY_OPERATOR(sin);})
DEF_CMD(cos,  24, ARG_NONE, "cosinus",     {UNARY_OPERATOR(cos);})



DEF_CMD(dmp,  31, ARG_NONE, "spu data dump", {
    DUMP();
})

DEF_CMD(shw,  32, ARG_IMM_DOUBLE,  "Graphics update", {
    SHOW();
    SLEEP((IMM_INT_T)GET_RVALUE());
})

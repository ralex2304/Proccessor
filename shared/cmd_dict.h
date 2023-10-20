#ifndef DEF_CMD
static_assert(0 && "DEF_CMD is not defined");
#endif

//    name | num | args | description | spu algorithm

// args: reg | imm_double | imm_int | ram | label

DEF_CMD(hlt,   0, 0b00000, "halt - end of program", {HALT();})

DEF_CMD(push,  1, 0b11111, "push one element to stack || take from reg and push ||"
                                       " take from reg + imm_double and push", {

    CHECK_AND_THROW_ERR(IS_ARG_REG || IS_ARG_IMM_DOUBLE || IS_ARG_IMM_INT,
                        "\"push\" requires at least one argument.");

    IMM_DOUBLE_T a = 0;

    if (IS_ARG_RAM) {
        IMM_INT_T b = 0;

        if (IS_ARG_REG)
            b += (IMM_INT_T)REG(ARG_REG);

        if (IS_ARG_IMM_INT)
            b += ARG_IMM_INT;

        a = RAM(b);
    } else {
        if (IS_ARG_REG)
            a += REG(ARG_REG);

        if (IS_ARG_IMM_DOUBLE)
            a += ARG_IMM_DOUBLE;

        if (IS_ARG_IMM_INT)
            a += (IMM_DOUBLE_T)ARG_IMM_INT;
    }

    PUSH(a);
})

DEF_CMD(pop,   2, 0b10110,  "pop from stack and write to reg", {
    CHECK_AND_THROW_ERR(IS_ARG_REG || IS_ARG_IMM_INT || IS_ARG_IMM_DOUBLE,
                        "\"pop\" requires reg.");

    if (IS_ARG_RAM) {
        IMM_INT_T a = 0;

        if (IS_ARG_REG)
            a += (IMM_INT_T)REG(ARG_REG);

        if (IS_ARG_IMM_INT)
            a += ARG_IMM_INT;

        POP(&RAM(a));
    } else {

    }

    POP(&REG(ARG_REG));
})

DEF_CMD(jmp,   3, 0b10001, "jump", {
    JUMP_CHECK_ARGS();

    JUMP(JUMP_DESTINATION());
})

DEF_CMD(ja,    4, 0b10001, "jump >", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_GREATER_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jae,   5, 0b10001, "jump >=", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_GREATER_EQUAL_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jb,    6, 0b10001, "jump <", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_LOWER_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jbe,   7, 0b10001, "jump <=", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_LOWER_EQUAL_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(je,    8, 0b10001, "jump ==", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(IS_EQUAL_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jne,   9, 0b10001, "jump !=", {
    JUMP_CHECK_ARGS();

    JUMP_CLAUSE_FUNC(!IS_EQUAL_IMM_DOUBLE, JUMP_DESTINATION());
})

DEF_CMD(jf,   10, 0b10001, "jump on Fridays", {
    JUMP_CHECK_ARGS();

    if (WEEKDAY() == 5)
        JUMP(JUMP_DESTINATION());
})

DEF_CMD(call, 11, 0b10001, "call - go to procedure", {
    JUMP_CHECK_ARGS();

    PUSH((IMM_DOUBLE_T)GET_NEXT_INSTRUCTION_PTR());

    JUMP(JUMP_DESTINATION());
})

DEF_CMD(ret,  12, 0b00000, "ret - end of procedure", {
    IMM_DOUBLE_T a = 0;

    POP(&a);

    IMM_INT_T b = (IMM_INT_T)a;

    JUMP(b);
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

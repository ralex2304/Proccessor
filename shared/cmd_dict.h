#ifndef DEF_CMD
static_assert(0 && "DEF_CMD is not defined");
#endif

DEF_CMD(HLT,   0, false, false, false, "halt - end of program", {HALT();})

DEF_CMD(push,  1, true,  true,  true,  "push one element to stack || take from reg and push ||"
                                       " take from reg + imm and push", {
    CHECK_AND_THROW_ERR(IS_ARG_REG || IS_ARG_IMM, "\"push\" requires at least one argument.");

    IMM_T a = 0;
    a += REG(ARG_REG);
    a += ARG_IMM;

    PUSH(a);
})

DEF_CMD(pop,   2, true,  false, true,  "pop from stack and write to reg", {
    CHECK_AND_THROW_ERR(IS_ARG_REG, "\"pop\" requires reg.");

    POP(&REG(ARG_REG));
})

DEF_CMD(in ,   3, false, false, false, "push one element to stack from user input", {
    IMM_T a = 0;
    GET_FROM_INPUT(&a);

    PUSH(a);
})

DEF_CMD(out,   4, false, false, false, "pop and print element from stack", {
    IMM_T a = 0;
    POP(&a);

    PRINT(a);
})

DEF_CMD(add,   5, false, false, false, "+",           {TWO_OPERANDS_POP_AND_PUSH(+);})
DEF_CMD(sub,   6, false, false, false, "-",           {TWO_OPERANDS_POP_AND_PUSH(-);})
DEF_CMD(mul,   7, false, false, false, "*",           {TWO_OPERANDS_POP_AND_PUSH(*);})
DEF_CMD(div,   8, false, false, false, "/",           {TWO_OPERANDS_POP_AND_PUSH(/);})
DEF_CMD(sqrt,  9, false, false, false, "sqaure root", {ONE_OPERAND_POP_AND_PUSH(sqrt);})
DEF_CMD(sin,  10, false, false, false, "sinus",       {ONE_OPERAND_POP_AND_PUSH(sin);})
DEF_CMD(cos,  11, false, false, false, "cosinus",     {ONE_OPERAND_POP_AND_PUSH(cos);})

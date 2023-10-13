/**
 * @brief Immutable var type
 */
#define IMM_T Imm_t

#define IS_ARG_REG cmd->byte.reg //< is reg given
#define IS_ARG_IMM cmd->byte.imm //< is imm given
#define IS_ARG_RAM cmd->byte.ram //< is ram given

#define ARG_REG      cmd->args.reg      //< reg num
#define ARG_IMM      cmd->args.imm      //< imm var
#define ARG_RAM_IMM  cmd->args.imm_ram  //< ram imm var

#define REG(num) spu->reg[num]          //< reg var by num

/**
 * @brief Halt function
 */
#define HALT() return Status::OK_EXIT

/**
 * @brief Checks condition and throws runtime error
 *
 * @param condition
 * @param error_msg
 */
#define CHECK_AND_THROW_ERR(condition, error_msg)   \
    if (!(condition))                               \
        THROW_RUNTIME_ERROR_(error_msg)

/**
 * @brief Pushes to stack
 *
 * @param var
 */
#define PUSH(var)                           \
    stk_res |= stk_push(&spu->stk, var);    \
    if (stk_res != Stack::OK) break

/**
 * @brief Pops from stack
 *
 * @param var pointer
 */
#define POP(var)                        \
    stk_res |= stk_pop(&spu->stk, var); \
    if (stk_res != Stack::OK) break

/**
 * @brief Gets imm to var from stdin
 *
 * @param var pointer
 */
#define GET_FROM_INPUT(var)                                 \
    *var = spu_get_imm_from_input();                        \
    if (!CHECK_IMM(*var)) return Status::WRONG_USER_INPUT

/**
 * @brief Prints imm to stdout
 *
 * @param var
 */
#define PRINT(var)                  \
    if (!spu_print_imm(a))          \
        return Status::OUTPUT_ERROR

/**
 * @brief Pops one element, pushes operator(A)
 *
 * @param operator
 */
#define ONE_OPERAND_POP_AND_PUSH(operator)  \
    IMM_T a = 0;                            \
    POP(&a);                                \
    PUSH(operator(a))

/**
 * @brief Pops two elements, pushes A operator B
 *
 * @param operator
 */
#define TWO_OPERANDS_POP_AND_PUSH(operator) \
    IMM_T a = 0, b = 0;                     \
    POP(&b);                                \
    POP(&a);                                \
    PUSH((a) operator (b))

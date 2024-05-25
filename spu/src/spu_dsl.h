/**
 * @brief Immutable var types
 */
#define IMM_DOUBLE_T    Imm_double_t
#define IMM_INT_T       Imm_int_t

#define IMM_DOUBLE_EPS  IMM_DOUBLE_T_EPSILON

#define IS_ARG_REG          cmd.keys.reg           //< is reg given
#define IS_ARG_IMM_INT      cmd.keys.imm_int       //< is int imm given
#define IS_ARG_IMM_DOUBLE   cmd.keys.imm_double    //< is double imm given
#define IS_ARG_RAM          cmd.keys.ram           //< is cmd type is ram

#define ARG_REG         cmd.args.reg           //< reg num
#define ARG_IMM_DOUBLE  cmd.args.imm_double    //< double imm var
#define ARG_IMM_INT     cmd.args.imm_int       //< int imm var

#define REG(num) spu->reg[num]      //< reg var by num
#define RAM(addr) spu->ram[addr]    //< ram value by address

/**
 * @brief Number comparison functions
 */
#define IS_EQUAL_IMM_DOUBLE(a, b)           (abs((a) - (b)) < IMM_DOUBLE_EPS)

#define IS_GREATER_IMM_DOUBLE(a, b)         ((a) > (b) && !IS_EQUAL_IMM_DOUBLE(a, b))

#define IS_LOWER_IMM_DOUBLE(a, b)           ((a) < (b) && !IS_EQUAL_IMM_DOUBLE(a, b))

#define IS_GREATER_EQUAL_IMM_DOUBLE(a, b)   ((a) > (b) || IS_EQUAL_IMM_DOUBLE(a, b))

#define IS_LOWER_EQUAL_IMM_DOUBLE(a, b)     ((a) < (b) || IS_EQUAL_IMM_DOUBLE(a, b))

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
 * @brief Returns current day of week
 *
 * @return int
 */
inline int WEEKDAY() {
    time_t t = time(NULL);
    tm *now = localtime(&t);
    return now->tm_wday;
}

inline IMM_DOUBLE_T get_rvalue_(SpuData* spu, const Cmd cmd) {
    IMM_DOUBLE_T ret = 0;

    if (IS_ARG_RAM) {
        IMM_INT_T imm_int = 0;

        if (IS_ARG_REG)
            imm_int += (IMM_INT_T)REG(ARG_REG);

        if (IS_ARG_IMM_INT)
            imm_int += ARG_IMM_INT;

        ret = RAM(imm_int);
    } else {
        if (IS_ARG_REG)
            ret += REG(ARG_REG);

        if (IS_ARG_IMM_DOUBLE)
            ret += ARG_IMM_DOUBLE;

        if (IS_ARG_IMM_INT)
            ret += (IMM_DOUBLE_T)ARG_IMM_INT;
    }

    return ret;
}

/**
 * @brief Gets rvalue
 */
#define GET_RVALUE() get_rvalue_(spu, cmd)

/**
 * @brief Checks args for GET_RVALUE()
 */
#define CHECK_GET_RVALUE_ARGS()                                                                         \
    CHECK_AND_THROW_ERR(!IS_ARG_RAM || IS_ARG_REG || IS_ARG_IMM_INT, "At least one argument required"); \
    CHECK_AND_THROW_ERR(!IS_ARG_RAM || !IS_ARG_IMM_DOUBLE, "Invalid set of arguments");                 \
    CHECK_AND_THROW_ERR( IS_ARG_RAM || IS_ARG_REG || IS_ARG_IMM_INT || IS_ARG_IMM_DOUBLE,               \
                                        "At least one arguments required")

inline IMM_DOUBLE_T* get_lvalue_ptr_(SpuData* spu, const Cmd cmd) {
    if (IS_ARG_RAM) {
        IMM_INT_T imm_int = 0;

        if (IS_ARG_REG)
            imm_int += (IMM_INT_T)REG(ARG_REG);

        if (IS_ARG_IMM_INT)
            imm_int += ARG_IMM_INT;

        return &RAM(imm_int);
    } else {
        return &REG(ARG_REG);
    }
}

/**
 * @brief Gets lvalue pointer
 */
#define GET_LVALUE_PTR() get_lvalue_ptr_(spu, cmd)

/**
 * @brief Checks args for GET_LVALUE_PTR()
 */
#define CHECK_GET_LVALUE_PTR_ARGS()                                                                         \
    CHECK_AND_THROW_ERR(!IS_ARG_RAM || IS_ARG_REG || IS_ARG_IMM_INT, "At least one arguments required.");   \
    CHECK_AND_THROW_ERR(!IS_ARG_RAM || !IS_ARG_IMM_DOUBLE, "Invalid set of arguments.");                    \
    CHECK_AND_THROW_ERR( IS_ARG_RAM || IS_ARG_REG, "Reg argument required");                                \
    CHECK_AND_THROW_ERR( IS_ARG_RAM || (!IS_ARG_IMM_INT && !IS_ARG_IMM_DOUBLE), "Invalid set of arguments.")

/**
 * @brief Halt function
 */
#define HALT() return Status::NORMAL_WORK

/**
 * @brief Spu data dump
 */
#define DUMP() spu->dump(data, cur_byte)

#ifdef GRAPHICS
/**
 * Updates graphics window
 */
#define SHOW() if (!spu_sfml_show(spu)) HALT()

/**
 * @brief Sets framerate limit
 *
 * @param fps
 */
#define SET_FPS(fps) spu_sfml_set_fps(spu, (unsigned int) fps);

#else //< #ifndef GRAPHICS

#define SHOW() THROW_RUNTIME_ERROR_("SHOW() command called, but graphics is disabled.");

#define SET_FPS(fps_) THROW_RUNTIME_ERROR_("SET_FPS() command called, but graphics is disabled.");

#endif //< #ifdef GRAPHICS

/**
 * @brief Sleeps time milliseconds
 *
 * @param time in milliseconds
 */
#define SLEEP(time) usleep((unsigned int)time * 1000)

/**
 * @brief Returns current instruction pointer in bytes
 */
#define GET_INSTRUCTION_PTR() (*cur_byte - cmd.size())

/**
 * @brief Returns next command instruction pointer in bytes
 */
#define GET_NEXT_INSTRUCTION_PTR() (*cur_byte)

/**
 * @brief Moves instruction pointer to specified address
 *
 * @param addr
 */
#define JUMP(addr) *cur_byte = addr

/**
 * @brief Pushes to stack
 *
 * @param var
 */
#define PUSH(var)                           \
    stk_res |= stk_push(&spu->stk, var);    \
    if (stk_res != Stack::OK) goto cmd_dispatch_do_stk_error;

/**
 * @brief Pops from stack
 *
 * @param var pointer
 */
#define POP(var)                        \
    stk_res |= stk_pop(&spu->stk, var); \
    if (stk_res != Stack::OK) goto cmd_dispatch_do_stk_error;

/**
 * @brief Gets imm to var from stdin
 *
 * @param var pointer
 */
#define GET_FROM_INPUT(var)                                 \
    *var = spu_get_imm_from_input();                        \
    if (!CHECK_IMM_DOUBLE(*var)) return Status::WRONG_USER_INPUT

/**
 * @brief Prints imm to stdout
 *
 * @param var
 */
#define PRINT(var)                  \
    if (!spu_print_imm(var))        \
        return Status::OUTPUT_ERROR

/**
 * @brief Pops one element, pushes operator(A)
 *
 * @param operator
 */
#define UNARY_OPERATOR(operator)    \
    IMM_DOUBLE_T a = 0;             \
    POP(&a);                        \
    PUSH(operator(a))

/**
 * @brief Pops two elements, pushes A operator B
 *
 * @param operator
 */
#define BINARY_OPERATOR(operator)   \
    IMM_DOUBLE_T a = 0, b = 0;      \
    POP(&b);                        \
    POP(&a);                        \
    PUSH((a) operator (b))

/**
 * @brief Pops two elements, pushes operator(A, B)
 *
 * @param operator
 */
#define BINARY_FUNC_OPERATOR(operator)  \
    IMM_DOUBLE_T a = 0, b = 0;          \
    POP(&b);                            \
    POP(&a);                            \
    PUSH(operator(a, b))

/**
 * @brief Pops two elements. Compares them with operator and jumps if true
 *
 * @param operator
 * @param addr
 */
#define JUMP_CLAUSE(operator, addr) \
    IMM_DOUBLE_T a = 0, b = 0;      \
    POP(&b);                        \
    POP(&a);                        \
    if ((a) operator (b))           \
        JUMP(addr);

/**
 * @brief Pops two elements. Gives them to func and jumps if true
 *
 * @param func bool function that recieves two arguments
 * @param addr
 */
#define JUMP_CLAUSE_FUNC(func, addr)    \
    IMM_DOUBLE_T a = 0, b = 0;          \
    POP(&b);                            \
    POP(&a);                            \
    if (func(a, b))                     \
        JUMP(addr);

inline IMM_INT_T jump_destination_(const SpuData* spu, const Cmd cmd) {
    IMM_INT_T imm_int = 0;
    if (IS_ARG_IMM_INT)
        imm_int += ARG_IMM_INT;

    if (IS_ARG_REG)
        imm_int += (IMM_INT_T)REG(ARG_REG);

    return imm_int;
}

/**
 * @brief Calculates jump destionation address
 */
#define JUMP_DESTINATION() jump_destination_(spu, cmd)

/**
 * @brief Checks args for JUMP_DESTINATION()
 */
#define CHECK_JUMP_DESTINATION_ARGS()                                                       \
    CHECK_AND_THROW_ERR(IS_ARG_IMM_INT || IS_ARG_REG, "At least one argument required.");   \
    CHECK_AND_THROW_ERR(!IS_ARG_RAM && !IS_ARG_IMM_DOUBLE, "Invalid set of arguments.")

#ifndef SPU_H_
#define SPU_H_

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include "cmd.h"
#include "lib/utils/statuses.h"
#include "stack/log/log.h"
#include "stack/stack.h"
#include "lib/utils/console.h"

/**
 * @brief Keeps spu structs and variables
 */
struct SpuData {
    Stack stk = {};                     //< Stack

    Imm_double_t reg[REGS_NUM] = {};    //< registers

    Status::Statuses ctor();
    Status::Statuses dtor();
};

/**
 * @brief Parses data and executes it
 *
 * @param data
 * @param size
 * @return Status::Statuses
 */
Status::Statuses spu_parse(const char* data, const size_t size);

/**
 * @brief Executes one command
 *
 * @param spu
 * @param cmd
 * @param cur_byte
 * @param op operation pointer
 * @return Status::Statuses
 */
Status::Statuses spu_execute_command(SpuData* spu, const Cmd* cmd, size_t* cur_byte,
                                     const size_t op);

/**
 * @brief Prints double to stdout
 *
 * @param x
 * @return true
 * @return false
 */
inline bool spu_print_imm(const double x) {
    return printf("%lf\n", x) > 0;
}

/**
 * @brief Flushes input
 *
 * @return int last read symbol (EOF if end of input reached)
 */
inline int spu_flush_input() {
    int c = '\0';
    while ((c = getchar()) != '\n' && c != EOF) {}
    return c;
}

/**
 * @brief Gets double from stdin
 *
 * @return double NAN if error
 */
inline double spu_get_imm_from_input() {
    double x = NAN;

    static const ssize_t MAX_INPUT_ATTEMPTS = 5;
    ssize_t attempts = MAX_INPUT_ATTEMPTS;
    while (attempts--) {
        printf("Enter double number: ");

        if (scanf("%lf", &x) == 1)
            return x;

        if (spu_flush_input() == EOF)
            break;

        printf("Wrong input. Try again\n");
    }

    printf("Too many wrong attempts\n");

    return NAN;
}

#endif // #ifndef SPU_H_

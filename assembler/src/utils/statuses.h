#ifndef STATUSES_H_
#define STATUSES_H_

#include <stdio.h>
#include <assert.h>

/**
 * @brief Specifies functions status
 */
struct Status {
    enum Statuses {
        DEFAULT        = -2,  ///< Poison value
        NORMAL_WORK    = -1,  ///< Normal mode system value (isn't returned in main())
        OK_EXIT        =  0,  ///< OK and exit
        ARGS_ERROR     =  1,  ///< Console arguments error
        INP_FILE_ERROR =  2,  ///< Input file opening or reading error
        OUT_FILE_ERROR =  3,  ///< Output file opening or writing error
        MEMORY_EXCEED  =  4,  ///< Memory allocation failed
        SYNTAX_ERROR   =  5,  ///< Syntax error in input file found
    };

    /**
     * @brief Prints and returns given status
     *
     * @param[in] status
     * @return Statuses
     */
    static Statuses raise(const Statuses status);
};

/**
 * @brief Macros for validating function return in main()
 *
 * @param func function
 */
#define STATUS_CHECK(func)  do {                                    \
                                Status::Statuses res = func;        \
                                if (res != Status::NORMAL_WORK)     \
                                    return Status::raise(res);      \
                            } while (0)


#endif // #ifndef STATUSES_H_

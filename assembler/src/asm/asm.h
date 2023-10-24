#ifndef ASM_H_
#define ASM_H_

#include "lib/utils/buf_lib.h"
#include "lib/text/text_lib.h"
#include "cmd.h"

const size_t ASM_PASSES_NUM = 2; //< number of assembly passes

/**
 * @brief Specifies jump cmd label data
 */
struct JumpLabel {
    String name = {nullptr, 0};
    ssize_t addr = -1;
};

/**
 * @brief Keeps asm code line data
 */
struct AsmLine {
    String text = {};                       //< current line text
    size_t num = 0;                         //< current line number

    Cmd cmd = {};                           //< command

    String tokens[MAX_LINE_LEN + 1] = {};   //< tokenised line
    size_t cur_token = 0;                   //< current line token index
    const char* comment = nullptr;          //< current line comment
};

/**
 * @brief Information about input and output files
 */
struct AsmFilesInfo {
    const char* inp_name = nullptr; //< input filename
    const char* bin_name = nullptr; //< binary output filename
    const char* lst_name = nullptr; //< listing output filename
};

/**
 * @brief Assembling data
 */
struct Asm {
    static const size_t LAST_PASS_NUM = ASM_PASSES_NUM - 1; //< number of last assembly pass

    bool ctor(const ssize_t buf_capacity);
    void dtor();

    Buffer buf = {};                        //< output buffer

    size_t pass_num = 0;                    //< asm pass number

    JumpLabel labels[MAX_LABEL_NUM] = {};   //< labels dictionary

    AsmFilesInfo files = {};                //< input and output files info
};

#endif // #ifndef ASM_H_

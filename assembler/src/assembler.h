#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <math.h>

#include "lib/utils/statuses.h"
#include "text/text_proccessor.h"
#include "lib/file/file.h"
#include "cmd.h"
#include "lib/utils/console.h"

/**
 * @brief Assembles code and writes it to binary or text file
 *
 * @param input_data
 * @param filename
 * @param debug_mode
 * @return Status::Statuses
 */
Status::Statuses assmeble_and_write(const InputData* input_data, const char* filename,
                                    bool debug_mode);

/**
 * @brief Parses and writes one line of code
 *
 * @param file
 * @param line
 * @param line_num
 * @param debug_mode
 * @return Status::Statuses
 */
Status::Statuses parse_and_write_line(FILE* file, String line, const size_t line_num,
                                      bool debug_mode);

/**
 * @brief Reads cmd reg from str
 *
 * @param str
 * @param cmd
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_reg(const char* str, Cmd* cmd, const size_t line_num);

/**
 * @brief Reads cmd imm from str
 *
 * @param str
 * @param cmd
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_imm(const char* str, Cmd* cmd, const size_t line_num);

/**
 * @brief Writes cmd in text mode
 *
 * @param file
 * @param cmd
 * @param binary_pos
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd_debug(FILE* file, const Cmd* cmd,
                                    const size_t binary_pos, const size_t line_num);

/**
 * @brief Writes cmd in binary mode
 *
 * @param file
 * @param cmd
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd_bin(FILE* file, const Cmd* cmd);

/**
 * @brief Writes command to file
 *
 * @param file
 * @param cmd
 * @param binary_pos
 * @param line_num
 * @param debug_mode
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd(FILE* file, const Cmd* cmd, size_t* binary_pos,
                               const size_t line_num, const bool debug_mode);

#endif // #ifndef ASSEMBLER_H_

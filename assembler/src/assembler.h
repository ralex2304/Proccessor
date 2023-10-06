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
 * @param cmd_byte
 * @param cmd_args
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_reg(const char* str, CmdByte* cmd_byte, CmdArgs* cmd_args,
                              const size_t line_num);

/**
 * @brief Reads cmd imm from str
 *
 * @param str
 * @param cmd_byte
 * @param cmd_args
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_read_imm(const char* str, CmdByte* cmd_byte, CmdArgs* cmd_args,
                              const size_t line_num);

/**
 * @brief Writes cmd in text mode
 *
 * @param file
 * @param cmd_byte
 * @param cmd_args
 * @param binary_pos
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd_text(FILE* file, const CmdByte* cmd_byte,
                                    const CmdArgs* cmd_args, const size_t binary_pos);

/**
 * @brief Writes cmd in binary mode
 *
 * @param file
 * @param cmd_byte
 * @param cmd_args
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd_bin(FILE* file, const CmdByte* cmd_byte,
                                   const CmdArgs* cmd_args);

/**
 * @brief Writes command to file
 *
 * @param file
 * @param cmd_byte
 * @param cmd_args
 * @param binary_pos
 * @param debug_mode
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd(FILE* file, const CmdByte* cmd_byte, const CmdArgs* cmd_args,
                               size_t* binary_pos, const bool debug_mode);

#endif // #ifndef ASSEMBLER_H_

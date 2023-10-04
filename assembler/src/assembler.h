#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <math.h>

#include "utils/statuses.h"
#include "text/text_proccessor.h"
#include "file/file.h"
#include "../../Commands.h"
#include "utils/console.h"

/**
 * @brief Assembles code and writes it to binary or text file
 *
 * @param input_data
 * @param filename
 * @param text_mode
 * @return Status::Statuses
 */
Status::Statuses assmeble_and_write(const InputData* input_data, const char* filename,
                                    bool text_mode);

/**
 * @brief Parses and writes one line of code
 *
 * @param file
 * @param line
 * @param line_num
 * @param text_mode
 * @return Status::Statuses
 */
Status::Statuses parse_and_write_line(FILE* file, String line, const size_t line_num,
                                      bool text_mode);

/**
 * @brief Finds command in COMMANDS[] by name
 *
 * @param name
 * @return const Command*
 */
const Command* find_command_by_name(const char* name);

/**
 * @brief Parses command argument (double)
 *
 * @param file
 * @param str
 * @param line_num
 * @param arg_num
 * @param text_mode
 * @param binary_offset
 * @return Status::Statuses
 */
Status::Statuses write_double_arg(FILE* file, const char* str, const size_t line_num,
                                  const size_t arg_num, const bool text_mode, size_t* binary_offset);

#endif // #ifndef ASSEMBLER_H_

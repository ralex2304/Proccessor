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
Status::Statuses assemeble_and_write(const InputData* input_data, const char* filename,
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
Status::Statuses asm_parse_and_write_line(FILE* file, String line, const size_t line_num,
                                          bool debug_mode);

/**
 * @brief Checks if [] are specified (ram arguments)
 *
 * @param str
 * @param cmd
 * @param line_num
 * @return Status::Statuses
 */
Status::Statuses asm_parse_is_arg_ram(char** str, Cmd* cmd, const size_t line_num);

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
 * @param comment
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd_debug(FILE* file, const Cmd* cmd,
                                    const size_t binary_pos, const size_t line_num,
                                    const char* comment);

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
 * @param comment
 * @return Status::Statuses
 */
Status::Statuses asm_write_cmd(FILE* file, const Cmd* cmd, size_t* binary_pos,
                               const size_t line_num, const bool debug_mode, const char* comment);

/**
 * @brief Writes signature to file
 *
 * @param file
 * @param debug_mode
 * @return Status::Statuses
 */
Status::Statuses asm_write_signature(FILE* file, const bool debug_mode);

/**
 * @brief Writes comment to listing file
 *
 * @param file
 * @param comment
 * @return true
 * @return false
 */
bool asm_write_debug_comment(FILE* file, const char* comment);

/**
 * @brief Calculates tab for debug listing
 *
 * @param cmd
 * @return size_t
 */
inline size_t asm_debug_calc_tab(const Cmd* cmd) {
    size_t res = 0;

    if (cmd == nullptr)
        res += sizeof(cmd->byte) * 3;

    if (cmd == nullptr || !cmd->byte.reg)
        res += sizeof(cmd->args.reg) * 3;

    if (cmd == nullptr || !cmd->byte.imm)
        res += MAX(sizeof(cmd->args.imm), sizeof(cmd->args.imm_ram)) * 3;

    if (cmd != nullptr && cmd->byte.imm)
        if (sizeof(cmd->args.imm) < sizeof(cmd->args.imm_ram))
            res += (sizeof(cmd->args.imm_ram) - sizeof(cmd->args.imm)) * 3;

    if (cmd != nullptr && cmd->byte.ram)
        if (sizeof(cmd->args.imm_ram) < sizeof(cmd->args.imm))
            res += (sizeof(cmd->args.imm) - sizeof(cmd->args.imm_ram)) * 3;

    return res;
}

#endif // #ifndef ASSEMBLER_H_

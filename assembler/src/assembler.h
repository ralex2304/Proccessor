#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <math.h>

#include "lib/utils/statuses.h"
#include "text/text_proccessor.h"
#include "lib/file/file.h"
#include "cmd.h"
#include "lib/utils/console.h"
#include "lib/utils/buf_lib.h"
#include "asm/jump.h"
#include "asm/read.h"
#include "asm/write.h"
#include "asm/listing.h"

/**
 * @brief Assembles code and writes it to binary file. Listing file is optional
 *
 * @param input_data
 * @param bin_filename
 * @param listing_filename
 * @return Status::Statuses
 */
Status::Statuses assemeble_and_write(const InputData* input_data, const char* bin_filename,
                                     const char* listing_filename = nullptr);

/**
 * @brief Parses one line and writes result to buf
 *
 * @param buf
 * @param line
 * @param labels
 * @param line_num
 * @param listing_file
 * @param first_pass
 * @return Status::Statuses
 */
Status::Statuses asm_parse_and_write_line(Buffer* buf, String line, JumpLabel* labels,
                                          const size_t line_num, FILE* listing_file,
                                          const bool first_pass);

/**
 * @brief Checks if cmd requires at least one argument
 *
 * @param cmd
 * @return true
 * @return false
 */
inline bool asm_is_arg_required(const Cmd* cmd) {
    return cmd->info->args.label || cmd->info->args.imm_int || cmd->info->args.imm_double
        || cmd->info->args.reg || cmd->info->args.ram;
}

#endif // #ifndef ASSEMBLER_H_

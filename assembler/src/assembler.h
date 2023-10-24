#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <math.h>

#include "lib/utils/statuses.h"
#include "lib/text/text_proccessor.h"
#include "lib/file/file.h"
#include "cmd.h"
#include "lib/utils/console.h"
#include "lib/utils/buf_lib.h"
#include "asm/jump.h"
#include "asm/read.h"
#include "asm/write.h"
#include "asm/listing.h"
#include "asm/errors.h"
#include "asm/asm.h"

/**
 * @brief Assembles code and writes it to binary file. Listing file is optional
 *
 * @param input_data
 * @param files
 * @return Status::Statuses
 */
Status::Statuses assemeble_and_write(const InputData* input_data, AsmFilesInfo files);

/**
 * @brief Does one assembly pass
 *
 * @param asm_data
 * @param input_data
 * @param lst_file
 * @return Status::Statuses
 */
Status::Statuses asm_do_pass(Asm* asm_data, const InputData* input_data, FILE* lst_file);

/**
 * @brief Parses one line and writes to buf
 *
 * @param asm_data
 * @param line
 * @param lst_file
 * @return Status::Statuses
 */
Status::Statuses asm_parse_and_write_line(Asm* asm_data, AsmLine* line, FILE* lst_file = nullptr);

#endif // #ifndef ASSEMBLER_H_

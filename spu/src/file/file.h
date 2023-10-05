#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include "../utils/statuses.h"

/**
 * @brief Opens file, reads binary data from it, closes file. Allocates memory
 *
 * @param filename
 * @param buf
 * @param file_len
 * @return Status::Statuses
 */
Status::Statuses file_open_read_bin_close(const char* filename, char** buf, long* file_len);

/**
 * @brief Opens file
 *
 * @param file
 * @param filename
 * @param mode
 * @return true success
 * @return false failure
 */
bool file_open(FILE** file, const char* filename, const char* mode);

/**
 * @brief Gets file len
 *
 * @param file
 * @return long -1 in case of error
 */
long file_get_len(FILE* file);

/**
 * @brief Reads data from char to buffer. Buffer must be big enough
 *
 * @param file
 * @param buf
 * @param file_len
 * @return true
 * @return false
 */
bool file_read(FILE* file, char* buf, long file_len);

/**
 * @brief Closes file
 *
 * @param file
 * @return true success
 * @return false failure
 */
bool file_close(FILE* file);

#endif // #ifndef FILE_H_

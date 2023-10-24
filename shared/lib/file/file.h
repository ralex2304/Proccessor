#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include "../utils/statuses.h"

/**
 * @brief Opens, reads and closes file. Allocates memory
 *
 * @param filename
 * @param buf
 * @param additional_memory will be allocated in addition to the file size
 * @param size readed file size
 * @return Status::Statuses
 */
Status::Statuses file_open_read_close(const char* filename, char** buf, const size_t additional_memory = 0,
                                      long* size = nullptr);

/**
 * @brief Opens, writes bytes and closes file
 *
 * @param filename
 * @param buf
 * @param size
 * @return Status::Statuses
 */
Status::Statuses file_open_write_bytes_close(const char* filename, char* buf, const size_t size);

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
 * @brief Writes line (string) to file
 *
 * @param file
 * @param line
 * @return true
 * @return false
 */
bool file_write_line(FILE* file, const char* line);

/**
 * @brief Writes data to file
 *
 * @param file
 * @param data
 * @param len data size in bytes
 * @return true
 * @return false
 */
bool file_write_bytes(FILE* file, const void* data, size_t len);

#ifdef __GNUG__ // because of __attribute__
/**
 * @brief Error handler for fprintf
 *
 * @param file
 * @param format
 * @param ...
 * @return int number of printed symbols or EOF
 */
int file_printf(FILE* file, const char* format, ...) __attribute__ ((format(printf, 2, 3)));
#else // #ifndef __GNUG__
/**
 * @brief Error handler for fprintf
 *
 * @param file
 * @param format
 * @param ...
 * @return int number of printed symbols or EOF
 */
int file_printf(FILE* file, const char* format, ...);
#endif // #ifdef __GNUG__

/**
 * @brief Closes file
 *
 * @param file
 * @return true success
 * @return false failure
 */
bool file_close(FILE* file);

#endif // #ifndef FILE_H_

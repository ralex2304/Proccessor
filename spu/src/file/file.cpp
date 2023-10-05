#include "file.h"

Status::Statuses file_open_read_bin_close(const char* filename, char** buf, long* file_len) {
    assert(filename);
    assert(buf);
    assert(*buf == nullptr);

    FILE* file = nullptr;
    if (!file_open(&file, filename, "rb"))
        return Status::INP_FILE_ERROR;

    *file_len = file_get_len(file);
    if (*file_len < 0) {
        file_close(file);
        return Status::INP_FILE_ERROR;
    }

    *buf = (char*)calloc(*file_len, sizeof(char));
    if (*buf == nullptr) {
        printf("Memory alloc error\n");
        file_close(file);
        return Status::MEMORY_EXCEED;
    }

    if (!file_read(file, *buf, *file_len)) {
        file_close(file);
        return Status::INP_FILE_ERROR;
    }

    if (!file_close(file))
        return Status::INP_FILE_ERROR;

    return Status::NORMAL_WORK;
}

bool file_open(FILE** file, const char* filename, const char* mode) {
    assert(file);
    assert(filename);
    assert(mode);

    *file = fopen(filename, mode);
    if (*file == nullptr) {
        printf("Error opening file %s:\n", filename);
        perror("");
        return false;
    }
    return true;
}

long file_get_len(FILE* file) {
    assert(file);

    long file_len = -1;

    if (fseek(file, 0, SEEK_END) != 0 ||
        (file_len = ftell(file)) == -1L ||
        fseek(file, 0, SEEK_SET) != 0) {

            perror("File len detection error");
            return -1;
    }

    return file_len;
}

bool file_read(FILE* file, char* buf, long file_len) {
    assert(file);
    assert(buf);

    size_t readed = fread(buf, sizeof(char), file_len, file);
    if (readed != (size_t)file_len || ferror(file)) {
        perror("File read error");
        return false;
    }

    return true;
}

bool file_close(FILE* file) {
    assert(file);

    if (fclose(file) != 0) {
        perror("File close error");
        return false;
    }

    return true;
}

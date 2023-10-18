#include "statuses.h"

Status::Statuses Status::raise(const Statuses status) {
    switch (status) {
        case NORMAL_WORK:
            assert(0 && "Status::raise(): NORMAL_WORK mustn't be raised");
            break;
        case ARGS_ERROR:
            fprintf(stderr, "Exiting. Args error\n");
            break;
        case INP_FILE_ERROR:
            fprintf(stderr, "Exiting. Input file error\n");
            break;
        case OUT_FILE_ERROR:
            fprintf(stderr, "Exiting. Output file error\n");
            break;
        case MEMORY_EXCEED:
            fprintf(stderr, "Exiting. Not enough memory\n");
            break;
        case SYNTAX_ERROR:
            fprintf(stderr, "Exiting. Syntax error\n");
            break;
        case RUNTIME_ERROR:
            fprintf(stderr, "Exiting. Runtime error\n");
            break;
        case WRONG_USER_INPUT:
            fprintf(stderr, "Exiting. Wrong user input\n");
            break;
        case OUTPUT_ERROR:
            fprintf(stderr, "Exiting. stdout error\n");
            break;
        case SIGNATURE_ERROR:
            fprintf(stderr, "Exiting. FileHeader error\n");
            break;
        case OK_EXIT:
        case DEFAULT:
            break;
        default:
            assert(0 && "Error::raise(): wrong error");
            break;
    };
    return status;
}

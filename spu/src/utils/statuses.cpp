#include "statuses.h"

Status::Statuses Status::raise(const Statuses status) {
    switch (status) {
        case NORMAL_WORK:
            assert(0 && "Status::raise(): NORMAL_WORK mustn't be raised");
            break;
        case ARGS_ERROR:
            printf("Exiting. Args error\n");
            break;
        case INP_FILE_ERROR:
            printf("Exiting. Input file error\n");
            break;
        case MEMORY_EXCEED:
            printf("Exiting. Not enough memory\n");
            break;
        case RUNTIME_ERROR:
            printf("Exiting. Runtime error\n");
            break;
        case WRONG_USER_INPUT:
            printf("Exiting. Wrong user input\n");
            break;
        case OUTPUT_ERROR:
            printf("Exiting. stdout error");
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

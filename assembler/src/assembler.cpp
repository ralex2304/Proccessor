#include "assembler.h"

Status::Statuses assmeble_and_write(const InputData* input_data, const char* filename,
                                    bool text_mode) {
    assert(input_data);
    assert(filename);

    FILE* file = {};
    if (!file_open(&file, filename, "wb"))
        return Status::OUT_FILE_ERROR;

    Status::Statuses res = Status::DEFAULT;

    for (size_t i = 0; i < input_data->lines_cnt; i++) {
        res = parse_and_write_line(file, input_data->lines[i], i + 1, text_mode);

        if (res != Status::NORMAL_WORK) {
            file_close(file);
            return res;
        }
    }

    if (!file_close(file))
        return Status::OUT_FILE_ERROR;

    return Status::NORMAL_WORK;
}

Status::Statuses parse_and_write_line(FILE* file, String line, const size_t line_num,
                                      bool text_mode) {
    assert(file);
    assert(line.str);

    static size_t binary_file_offset = 0;
    char command_name[MAX_LINE_LEN] = {};

    line.str = strtok(line.str, " ");
    int res = sscanf(line.str, "%s", command_name);

    if (res != 1) {
        printf(CONSOLE_RED("Syntax error.") " Line %zu\n", line_num);
        return Status::SYNTAX_ERROR;
    }

    const Command* command = find_command_by_name(command_name);
    if (command == nullptr) {
        printf(CONSOLE_RED("Syntax error. Command not found.") " Line %zu\n", line_num);
        return Status::SYNTAX_ERROR;
    }

    if (!text_mode)
        res = file_write_bytes(file, &command->num, sizeof(command->num));
    else
        res = file_printf(file, "/*%5zu*/ %d", binary_file_offset, command->num);

    if (!res)
        return Status::OUT_FILE_ERROR;

    binary_file_offset += sizeof(command->num);

    Status::Statuses write_res = Status::DEFAULT;

    for (size_t i = 0; i < COMMAND_MAX_ARGS_COUNT && command->args[i] != ArgType::NONE; i++) {
        line.str = strtok(nullptr, " ");

        if (line.str == nullptr) {
            printf(CONSOLE_RED("Syntax error. Argument %zu not found.") " Line %zu\n", i + 1, line_num);
            return Status::SYNTAX_ERROR;
        }

        switch (command->args[i]) {
            case ArgType::DOUBLE:
                write_res = write_double_arg(file, line.str,
                                             line_num, i + 1, text_mode, &binary_file_offset);
                break;
            case ArgType::NONE:
            default:
                assert(0 && "wrong ArgType is set in command");
                break;
        }

        if (write_res != Status::NORMAL_WORK)
            return write_res;
    }

    if (text_mode)
        file_write_line(file, "\n");

    return Status::NORMAL_WORK;
}

const Command* find_command_by_name(const char* name) {
    assert(name);

    for (size_t i = 0; i < COMMANDS_NUM; i++) {
        if (strcmp(COMMANDS[i].name, name) == 0)
            return COMMANDS + i;
    }

    return nullptr;
}

Status::Statuses write_double_arg(FILE* file, const char* str, const size_t line_num,
                                  const size_t arg_num, const bool text_mode, size_t* binary_offset) {
    assert(file);
    assert(str);

    double arg = NAN;

    int res = sscanf(str, "%lf", &arg);

    if (res != 1 || isnan(arg)) {
        printf(CONSOLE_RED("Syntax error. Wrong argument %zu.") " Line %zu\n", arg_num, line_num);
        return Status::SYNTAX_ERROR;
    }

    if (!text_mode)
        res = file_write_bytes(file, &arg, sizeof(arg));
    else
        res = file_printf(file, " %lf", arg);

    if (!res)
        return Status::OUT_FILE_ERROR;

    *binary_offset += sizeof(arg);

    return Status::NORMAL_WORK;
}

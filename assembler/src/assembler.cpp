#include "assembler.h"


#define LOCAL_DTOR_()   do {            \
                            buf.dtor(); \
                        } while(0)

#define FILE_CHECK_(func_, ...)  do {                                   \
                                    if (!func_) {                       \
                                        __VA_ARGS__;                    \
                                        return Status::OUT_FILE_ERROR;  \
                                    }                                   \
                                } while(0)

Status::Statuses assemeble_and_write(const InputData* input_data, const char* bin_filename,
                                     const char* listing_filename) {
    assert(input_data);
    assert(bin_filename);

    FILE* listing_file = nullptr;

    Buffer buf = {};
    if (!buf.ctor())
        return Status::MEMORY_EXCEED;

    JumpLabel labels[MAX_LABEL_NUM] = {};

    STATUS_CHECK(asm_write_header(&buf, listing_file, true), LOCAL_DTOR_());

    for (size_t i = 0; i < input_data->lines_cnt; i++) {
        STATUS_CHECK(asm_parse_and_write_line(&buf, input_data->lines[i],
                                              labels, i + 1, listing_file, true),
                     LOCAL_DTOR_());
    }

    // Second pass

    if (listing_filename != nullptr)
        FILE_CHECK_(file_open(&listing_file, listing_filename, "wb"), LOCAL_DTOR_());

    buf.size = 0;

    STATUS_CHECK(asm_write_header(&buf, listing_file, false), LOCAL_DTOR_());

    if (listing_filename != nullptr)
        if (file_printf(listing_file, ";line - byte - command\n") == EOF)
            return Status::OUT_FILE_ERROR;

    for (size_t i = 0; i < input_data->lines_cnt; i++) {
        STATUS_CHECK(asm_parse_and_write_line(&buf, input_data->lines[i],
                                              labels, i + 1, listing_file, false),
                     LOCAL_DTOR_());
    }

    if (listing_filename != nullptr)
        FILE_CHECK_(file_close(listing_file), buf.dtor());

    // Binary file write

    STATUS_CHECK(file_open_write_bytes_close(bin_filename, buf.data, buf.size), LOCAL_DTOR_());

    return Status::NORMAL_WORK;
}
#undef LOCAL_DTOR_


#define LOCAL_DTOR_()   do {            \
                            FREE(str);  \
                        } while(0)

#define THROW_SYNTAX_ERROR_(text, ...)  do {                                                        \
            fprintf(stderr, CONSOLE_RED("Syntax error. " text) " Line %zu\n", __VA_ARGS__ line_num);\
            LOCAL_DTOR_();                                                                          \
            return Status::SYNTAX_ERROR;                                                            \
        } while(0)

Status::Statuses asm_parse_and_write_line(Buffer* buf, String line, JumpLabel* labels,
                                          const size_t line_num, FILE* listing_file,
                                          const bool first_pass) {
    assert(buf);
    assert(line.str);
    assert(labels);

    Cmd cmd = {};

    char* str = strdup(line.str);
    if (str == nullptr)
        return Status::MEMORY_EXCEED;

    const char* comment = text_throw_out_comment(str);

    const char* special_delims = "[]:";

    const char* tokens[MAX_LINE_LEN + 1] = {};

    split_line_with_special_delims(str, " +\t", special_delims, tokens, MAX_LINE_LEN);

    size_t cur_token = 0;

    // Empty line check
    if (tokens[cur_token] == nullptr || tokens[cur_token][0] == '\0') {
        STATUS_CHECK(asm_write_cmd(buf, nullptr, labels, line_num,
                                   listing_file, first_pass, ';', comment));

        return Status::NORMAL_WORK;
    }

    // Parsing start

    if (strchr(special_delims, tokens[cur_token][0]) != nullptr)
        THROW_SYNTAX_ERROR_("Unexpected '%c' found.", tokens[cur_token][0],);

    cmd.info = find_command_by_name(tokens[cur_token]);
    if (cmd.info == nullptr) {
        if (tokens[cur_token + 1][0] == ':') {
            if (first_pass)
                STATUS_CHECK(asm_new_label(labels, tokens[cur_token], buf->size, line_num));

            STATUS_CHECK(asm_write_cmd(buf, nullptr, labels, line_num,
                                       listing_file, first_pass, '\0', str_skip_spaces(line.str)));
            return Status::NORMAL_WORK;
        } else
            THROW_SYNTAX_ERROR_("Command \"%s\" not found.", tokens[cur_token],);
    }

    cmd.byte.num = cmd.info->num & CMD_BYTE_NUM_BIT_MASK;

    cur_token += 1;

    if (tokens[cur_token] != nullptr) {
        if (strchr(special_delims, tokens[cur_token][0]) != nullptr) {
            if (tokens[cur_token][0] == '[') {
                cur_token++;
                STATUS_CHECK(asm_read_args_ram(tokens, &cur_token, &cmd, line_num));
            } else
                THROW_SYNTAX_ERROR_("Unexpected '%c' found.", tokens[cur_token][0],);
        } else {
            STATUS_CHECK(asm_read_args(tokens, &cur_token, &cmd, labels, line_num, first_pass));
        }

        if (tokens[cur_token] != nullptr)
            THROW_SYNTAX_ERROR_("Unexpected arg given.");
    }

    if (asm_is_arg_required(&cmd) && !(cmd.byte.imm || cmd.byte.ram || cmd.byte.reg))
        THROW_SYNTAX_ERROR_("At least one argument required.");

    STATUS_CHECK(asm_write_cmd(buf, &cmd, labels, line_num,
                               listing_file, first_pass, ';', comment));

    LOCAL_DTOR_();

    return Status::NORMAL_WORK;
}
#undef LOCAL_DTOR_
#undef THROW_SYNTAX_ERROR_

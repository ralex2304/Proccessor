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
                                     const char* inp_filename, const char* listing_filename) {
    assert(input_data);
    assert(bin_filename);
    assert(inp_filename);

    FILE* listing_file = nullptr;

    AsmInfo asm_info = {{}, 0, nullptr, inp_filename, false};

    Buffer buf = {};
    if (!buf.ctor())
        return Status::MEMORY_EXCEED;

    JumpLabel labels[MAX_LABEL_NUM] = {};

    STATUS_CHECK(asm_write_header(&buf, listing_file, false), LOCAL_DTOR_());

    for (size_t i = 0; i < input_data->lines_cnt; i++) {
        asm_info.line_num = i + 1;
        asm_info.line = input_data->lines[i];

        STATUS_CHECK(asm_parse_and_write_line(&buf, labels, &asm_info, listing_file),
                     LOCAL_DTOR_());
    }

    // Second pass
    asm_info.final_pass = true;

    if (listing_filename != nullptr)
        FILE_CHECK_(file_open(&listing_file, listing_filename, "wb"), LOCAL_DTOR_());

    buf.size = 0;

    STATUS_CHECK(asm_write_header(&buf, listing_file, asm_info.final_pass), LOCAL_DTOR_());

    if (listing_filename != nullptr)
        if (file_printf(listing_file, ";line - byte - command\n") == EOF)
            return Status::OUT_FILE_ERROR;

    for (size_t i = 0; i < input_data->lines_cnt; i++) {
        asm_info.line_num = i + 1;
        asm_info.line = input_data->lines[i];

        STATUS_CHECK(asm_parse_and_write_line(&buf, labels, &asm_info, listing_file),
                     LOCAL_DTOR_());
    }

    if (listing_filename != nullptr)
        FILE_CHECK_(file_close(listing_file), LOCAL_DTOR_());

    // Binary file write

    STATUS_CHECK(file_open_write_bytes_close(bin_filename, buf.data, buf.size), LOCAL_DTOR_());

    LOCAL_DTOR_();

    return Status::NORMAL_WORK;
}
#undef LOCAL_DTOR_


#define THROW_SYNTAX_ERROR_(...)  return asm_throw_syntax_error(tokens[cur_token],  \
                                                                asm_info,           \
                                                                __VA_ARGS__)

Status::Statuses asm_parse_and_write_line(Buffer* buf, JumpLabel* labels, AsmInfo* asm_info,
                                          FILE* listing_file) {
    assert(buf);
    assert(asm_info);
    assert(asm_info->filename);
    assert(asm_info->line.str);
    assert(labels);

    Cmd cmd = {};

    asm_info->comment = strchr(asm_info->line.str, ';');

    const char* special_delims = "[]:";

    String tokens[MAX_LINE_LEN + 1] = {};

    split_line_with_special_delims(asm_info->line.str, " +\t", special_delims, ";",
                                   tokens, MAX_LINE_LEN /*tokens array size*/);

    size_t cur_token = 0;

    // Empty line check
    if (tokens[cur_token].len == 0) {
        STATUS_CHECK(asm_write_cmd(buf, nullptr, labels, asm_info,
                                   listing_file));

        return Status::NORMAL_WORK;
    }

    // Parsing start

    if (strchr(special_delims, tokens[cur_token].str[0]) != nullptr)
        THROW_SYNTAX_ERROR_("Unexpected '%c' found.", tokens[cur_token].str[0]);

    cmd.info = find_command_by_name(tokens[cur_token]);
    if (cmd.info == nullptr) {
        // Label parsing
        if (tokens[cur_token + 1].len == 1 && tokens[cur_token + 1].str[0] == ':') {
            if (!asm_info->final_pass)
                STATUS_CHECK(asm_new_label(labels, tokens[cur_token], asm_info, buf->size));
            cur_token += 2;

            asm_info->comment = str_skip_spaces(asm_info->line.str);
            STATUS_CHECK(asm_write_cmd(buf, nullptr, labels, asm_info, listing_file));

            if (tokens[cur_token].str != nullptr)
                THROW_SYNTAX_ERROR_("Unexpected arg given.");

            return Status::NORMAL_WORK;
        } else
            THROW_SYNTAX_ERROR_("Command \"%.*s\" not found.", String_PRINTF(tokens[cur_token]));
    }

    cmd.keys.num = cmd.info->num & CMD_BYTE_NUM_BIT_MASK;

    cur_token += 1;

    if (tokens[cur_token].str != nullptr && tokens[cur_token].len != 0) {
        STATUS_CHECK(asm_read_args(tokens, &cur_token, &cmd, asm_info, labels));

        if (tokens[cur_token].str != nullptr) //< read_args increments cur_token
            THROW_SYNTAX_ERROR_("Unexpected arg given.");
    }

    if (asm_is_arg_required(&cmd) && !asm_is_any_arg_given(&cmd))
        THROW_SYNTAX_ERROR_("At least one argument required.");

    STATUS_CHECK(asm_write_cmd(buf, &cmd, labels, asm_info, listing_file));

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

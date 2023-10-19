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

    InputFileInfo inp_file = {{}, 0, nullptr, inp_filename};

    Buffer buf = {};
    if (!buf.ctor())
        return Status::MEMORY_EXCEED;

    JumpLabel labels[MAX_LABEL_NUM] = {};

    STATUS_CHECK(asm_write_header(&buf, listing_file, true), LOCAL_DTOR_());

    for (size_t i = 0; i < input_data->lines_cnt; i++) {
        inp_file.line_num = i + 1;
        inp_file.line = input_data->lines[i];

        STATUS_CHECK(asm_parse_and_write_line(&buf, labels, &inp_file, false, listing_file),
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
        inp_file.line_num = i + 1;
        inp_file.line = input_data->lines[i];

        STATUS_CHECK(asm_parse_and_write_line(&buf, labels, &inp_file, true, listing_file),
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
                                                                inp_file,           \
                                                                __VA_ARGS__)

Status::Statuses asm_parse_and_write_line(Buffer* buf, JumpLabel* labels, InputFileInfo* inp_file,
                                          const bool final_pass, FILE* listing_file) {
    assert(buf);
    assert(inp_file);
    assert(inp_file->filename);
    assert(inp_file->line.str);
    assert(labels);

    Cmd cmd = {};

    inp_file->comment = strchr(inp_file->line.str, ';');

    const char* special_delims = "[]:";

    String tokens[MAX_LINE_LEN + 1] = {};

    split_line_with_special_delims(inp_file->line.str, " +\t", special_delims, ";",
                                   tokens, MAX_LINE_LEN /*tokens array size*/);

    size_t cur_token = 0;

    // Empty line check
    if (tokens[cur_token].len == 0) {
        STATUS_CHECK(asm_write_cmd(buf, nullptr, labels, inp_file,
                                   listing_file, final_pass));

        return Status::NORMAL_WORK;
    }

    // Parsing start

    if (strchr(special_delims, tokens[cur_token].str[0]) != nullptr)
        THROW_SYNTAX_ERROR_("Unexpected '%c' found.", tokens[cur_token].str[0]);

    cmd.info = find_command_by_name(tokens[cur_token]);
    if (cmd.info == nullptr) {
        if (tokens[cur_token + 1].len >= 1 && tokens[cur_token + 1].str[0] == ':') {
            if (!final_pass)
                STATUS_CHECK(asm_new_label(labels, tokens[cur_token], inp_file, buf->size));
            cur_token += 2;

            inp_file->comment = str_skip_spaces(inp_file->line.str);
            STATUS_CHECK(asm_write_cmd(buf, nullptr, labels, inp_file,
                                       listing_file, final_pass));

            if (tokens[cur_token].str != nullptr)
                THROW_SYNTAX_ERROR_("Unexpected arg given.");
            return Status::NORMAL_WORK;
        } else
            THROW_SYNTAX_ERROR_("Command \"%.*s\" not found.", tokens[cur_token].len,
                                                               tokens[cur_token].str);
    }

    cmd.byte.num = cmd.info->num & CMD_BYTE_NUM_BIT_MASK;

    cur_token += 1;

    if (tokens[cur_token].str != nullptr && tokens[cur_token].len != 0) {
        if (strchr(special_delims, tokens[cur_token].str[0]) != nullptr) {
            if (tokens[cur_token].str[0] == '[') {
                cur_token++;
                STATUS_CHECK(asm_read_args_ram(tokens, &cur_token, &cmd, inp_file));
            } else
                THROW_SYNTAX_ERROR_("Unexpected '%c' found.", tokens[cur_token].str[0]);
        } else {
            STATUS_CHECK(asm_read_args(tokens, &cur_token, &cmd, inp_file, labels, final_pass));
        }

        if (tokens[cur_token].str != nullptr) //< read_args increments cur_token
            THROW_SYNTAX_ERROR_("Unexpected arg given.");
    }

    if (asm_is_arg_required(&cmd) && !(cmd.byte.imm || cmd.byte.ram || cmd.byte.reg))
        THROW_SYNTAX_ERROR_("At least one argument required.");

    STATUS_CHECK(asm_write_cmd(buf, &cmd, labels, inp_file, listing_file, final_pass));

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

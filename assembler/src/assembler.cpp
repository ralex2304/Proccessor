#include "assembler.h"


#define LOCAL_DTOR_()   do {                \
                            asm_data.dtor();\
                        } while(0)

#define FILE_CHECK_(func_, ...)  do {                                   \
                                    if (!func_) {                       \
                                        __VA_ARGS__;                    \
                                        return Status::OUT_FILE_ERROR;  \
                                    }                                   \
                                } while(0)

Status::Statuses assemeble_and_write(const InputData* input_data, const AsmFilesInfo files) {
    assert(input_data);
    assert(files.inp_name);
    assert(files.bin_name);
    // listing filename may be nullptr

    FILE* lst_file = nullptr;

    Asm asm_data = {};
    asm_data.files = files;

    if (!asm_data.ctor(-1)) // buf ctor is not needed. Will be initialised after first pass
        return Status::MEMORY_EXCEED;


    // First pass
    asm_data.pass_num = 0;
    STATUS_CHECK(asm_do_pass(&asm_data, input_data, lst_file), LOCAL_DTOR_());

    if (!asm_data.buf.ctor(asm_data.buf.capacity))
        return Status::MEMORY_EXCEED;


    // Second pass
    asm_data.pass_num = 1;
    STATUS_CHECK(asm_do_pass(&asm_data, input_data, lst_file), LOCAL_DTOR_());


    // Binary file write
    STATUS_CHECK(file_open_write_bytes_close(asm_data.files.bin_name,
                                             asm_data.buf.data, asm_data.buf.size), LOCAL_DTOR_());

    LOCAL_DTOR_();

    return Status::NORMAL_WORK;
}
#undef LOCAL_DTOR_


#define THROW_SYNTAX_ERROR_(...)  return asm_throw_syntax_error(asm_data, line, __VA_ARGS__)

Status::Statuses asm_parse_and_write_line(Asm* asm_data, AsmLine* line, FILE* lst_file) {
    assert(asm_data);
    assert(line);

    line->comment = strchr(line->text.str, ';');

    const char* special_delims = "[]:";

    split_line_with_special_delims(line->text.str, " +\t", special_delims, ";",
                                   line->tokens, MAX_LINE_LEN /*tokens array size*/);

    STATUS_CHECK(asm_read_cmd(asm_data, line));

    STATUS_CHECK(asm_read_args(asm_data, line));

    if (line->tokens[line->cur_token].len != 0) //< read_args increments cur_token
        THROW_SYNTAX_ERROR_("Unexpected arg given.");

    if (cmd_is_arg_required(&line->cmd) && !cmd_is_any_arg_given(&line->cmd))
        THROW_SYNTAX_ERROR_("At least one argument required.");

    STATUS_CHECK(asm_write_cmd(asm_data, line, lst_file));

    return Status::NORMAL_WORK;
}
#undef THROW_SYNTAX_ERROR_

Status::Statuses asm_do_pass(Asm* asm_data, const InputData* input_data, FILE* lst_file) {
    assert(asm_data);
    assert(input_data);
    // lst_file may be nullptr

    if (asm_data->pass_num == asm_data->LAST_PASS_NUM && asm_data->files.lst_name != nullptr)
        FILE_CHECK_(file_open(&lst_file, asm_data->files.lst_name, "wb"));

    STATUS_CHECK(asm_write_header(&asm_data->buf, lst_file,
                                   asm_data->pass_num == asm_data->LAST_PASS_NUM));

    if (asm_data->pass_num == asm_data->LAST_PASS_NUM && asm_data->files.lst_name != nullptr)
        if (file_printf(lst_file, ";line | addr | command\n") == EOF)
            return Status::OUT_FILE_ERROR;

    for (size_t i = 0; i < input_data->lines_cnt; i++) {
        AsmLine line = {};
        line.num = i + 1;
        line.text = input_data->lines[i];

        STATUS_CHECK(asm_parse_and_write_line(asm_data, &line, lst_file));
    }

    if (asm_data->pass_num == asm_data->LAST_PASS_NUM && asm_data->files.lst_name != nullptr)
        FILE_CHECK_(file_close(lst_file));

    return Status::NORMAL_WORK;
}

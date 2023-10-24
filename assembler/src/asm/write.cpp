#include "write.h"

Status::Statuses asm_write_cmd(Asm* asm_data, const AsmLine* line, FILE* listing_file) {
    assert(asm_data);
    assert(line);

    if (asm_data->pass_num == asm_data->LAST_PASS_NUM && listing_file != nullptr)
        STATUS_CHECK(asm_write_cmd_listing(listing_file, asm_data, line));

    STATUS_CHECK(asm_write_cmd_bin(&asm_data->buf, &line->cmd,
                                    asm_data->pass_num == asm_data->LAST_PASS_NUM));

    return Status::NORMAL_WORK;
}


#define BUF_CAT_(data_) do {                                                        \
                            if (final_pass) {                                       \
                                if (!buf->cat((const char*)&data_, sizeof(data_)))  \
                                    return Status::MEMORY_EXCEED;                   \
                            } else                                                  \
                                buf->capacity += sizeof(data_);                     \
                            } while (0)

Status::Statuses asm_write_cmd_bin(Buffer* buf, const Cmd* cmd, const bool final_pass) {
    assert(buf);

    if (cmd->info == nullptr)
        return Status::NORMAL_WORK;

    BUF_CAT_(cmd->keys);

    if (cmd->keys.reg)
        BUF_CAT_(cmd->args.reg);

    if (cmd->keys.imm_double)
        BUF_CAT_(cmd->args.imm_double);

    if (cmd->keys.imm_int)
        BUF_CAT_(cmd->args.imm_int);

    return Status::NORMAL_WORK;
}


#define F_PRINTF_CHECK_(printf)  if (printf == EOF) return Status::OUT_FILE_ERROR
#define F_WRITE_CHECK_(write)    if (!write)        return Status::OUT_FILE_ERROR

Status::Statuses asm_write_header(Buffer* buf, FILE* listing_file, const bool final_pass) {
    assert(buf);

    if (final_pass && listing_file != nullptr) {
        STATUS_CHECK(asm_write_header_listing(listing_file));
    }

    STATUS_CHECK(asm_write_header_bin(buf, final_pass)); //< Bin write after listing!!! (changes cur_byte)

    return Status::NORMAL_WORK;
}

Status::Statuses asm_write_header_bin(Buffer* buf, const bool final_pass) {
    assert(buf);

    BUF_CAT_(FILE_HEADER);

    return Status::NORMAL_WORK;
}

#undef F_WRITE_CHECK_
#undef F_PRINTF_CHECK_
#undef BUF_CHECK_


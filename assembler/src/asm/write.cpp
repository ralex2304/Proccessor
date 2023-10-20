#include "write.h"

Status::Statuses asm_write_cmd(Buffer* buf, const Cmd* cmd, JumpLabel* labels,
                               const AsmInfo* asm_info, FILE* listing_file) {
    assert(buf);
    assert(labels);
    assert(asm_info);
    // cmd assertion is not needed

    if (asm_info->final_pass && listing_file != nullptr)
        STATUS_CHECK(asm_write_cmd_listing(listing_file, cmd, labels, asm_info, buf->size));

    STATUS_CHECK(asm_write_cmd_bin(buf, cmd));

    return Status::NORMAL_WORK;
}


#define BUF_CAT_(data_) if (!buf->cat((const char*)&data_, sizeof(data_))) \
                            return Status::MEMORY_EXCEED

Status::Statuses asm_write_cmd_bin(Buffer* buf, const Cmd* cmd) {
    assert(buf);

    if (cmd == nullptr)
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

    STATUS_CHECK(asm_write_header_bin(buf)); //< Bin write after listing!!! (changes cur_byte)

    return Status::NORMAL_WORK;
}

Status::Statuses asm_write_header_bin(Buffer* buf) {
    assert(buf);

    BUF_CAT_(FILE_HEADER);

    return Status::NORMAL_WORK;
}
#undef F_WRITE_CHECK_
#undef F_PRINTF_CHECK_
#undef BUF_CHECK_


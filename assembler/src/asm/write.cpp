#include "write.h"

Status::Statuses asm_write_cmd(Buffer* buf, const Cmd* cmd, JumpLabel* labels,
                               const InputFileInfo* inp_file,
                               FILE* listing_file, const bool final_pass) {
    assert(buf);
    assert(labels);
    assert(inp_file);
    // cmd assertion is not needed

    if (final_pass && listing_file != nullptr)
        STATUS_CHECK(asm_write_cmd_listing(listing_file, cmd, labels, inp_file, buf->size));

    STATUS_CHECK(asm_write_cmd_bin(buf, cmd));

    return Status::NORMAL_WORK;
}


#define BUF_CAT_(data_) if (!buf->cat((const char*)&data_, sizeof(data_))) \
                            return Status::MEMORY_EXCEED

Status::Statuses asm_write_cmd_bin(Buffer* buf, const Cmd* cmd) {
    assert(buf);

    if (cmd == nullptr)
        return Status::NORMAL_WORK;

    BUF_CAT_(cmd->byte);

    if (cmd->byte.reg)
        BUF_CAT_(cmd->args.reg);

    if (cmd->byte.imm) {
        if (cmd->info->args.label || cmd->byte.ram) {
            BUF_CAT_(cmd->args.imm_int);
        } else {
            BUF_CAT_(cmd->args.imm_double);
        }
    }

    return Status::NORMAL_WORK;
}


#define F_PRINTF_CHECK_(printf)  if (printf == EOF) return Status::OUT_FILE_ERROR
#define F_WRITE_CHECK_(write)    if (!write)        return Status::OUT_FILE_ERROR

Status::Statuses asm_write_header(Buffer* buf, FILE* listing_file, const bool first_pass) {
    assert(buf);

    if (!first_pass && listing_file != nullptr) {
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


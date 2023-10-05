#include "utils/statuses.h"
#include "utils/args_parser.h"
#include "file/file.h"
#include "utils/macros.h"
#include "spu.h"

int main(int argc, char* argv[]) {

    /// Parsing console arguments
    ArgsVars args_vars = {};
    args_vars.input_filename  = "../main.exec"; ///< default value

    STATUS_CHECK(args_parse(argc, argv, &args_vars));
    /// Parsing console arguments end

    /// Reading input data
    char* inp_data = nullptr;
    long size = 0;
    STATUS_CHECK(file_open_read_bin_close(args_vars.input_filename, &inp_data, &size));
    /// Reading input data end

    /// Parsing and executing input file
    Status::Statuses exec_res = spu_parse(inp_data, size);
    /// Parsing and executing input file end

    FREE(inp_data);

    return Status::raise(exec_res); // raising, because exec_res may return error
}

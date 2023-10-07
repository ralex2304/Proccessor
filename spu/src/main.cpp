#include "lib/utils/statuses.h"
#include "lib/utils/args_parser.h"
#include "lib/file/file.h"
#include "lib/utils/macros.h"
#include "spu.h"

/**
 * @brief This array contains console options, their functions and descriptions
 */
const Argument ARGS_DICT[] = {
    {"-h", print_help,           "#   -h - prints help information\n"},             //< Help option

    {"-i", read_input_filename,  "#   -i - specify input file name after this\n"},  //< Input filename
};

const int ARGS_DICT_LEN = sizeof(ARGS_DICT) / sizeof(ARGS_DICT[0]);                 //< args_dict array len

int main(int argc, char* argv[]) {

    /// Parsing console arguments
    ArgsVars args_vars = {};
    args_vars.input_filename  = "../main.exec"; //< default value

    STATUS_CHECK_RAISE(args_parse(argc, argv, &args_vars, ARGS_DICT, ARGS_DICT_LEN));
    /// Parsing console arguments end

    /// Reading input data
    char* inp_data = nullptr;
    long size = 0;
    STATUS_CHECK_RAISE(file_open_read_close(args_vars.input_filename, &inp_data, 0, &size),
                       FREE(inp_data););
    /// Reading input data end

    /// Parsing and executing input file
    STATUS_CHECK_RAISE(spu_parse(inp_data, size),
                       FREE(inp_data));
    /// Parsing and executing input file end

    FREE(inp_data);

    return Status::OK_EXIT;
}

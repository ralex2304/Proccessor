#include "lib/utils/statuses.h"
#include "lib/utils/args_parser.h"
#include "lib/file/file.h"
#include "lib/utils/macros.h"
#include "disassembler.h"

/**
 * @brief This array contains console options, their functions and descriptions
 */
const Argument ARGS_DICT[] = {
    {"-h", print_help,           "#   -h - prints help information\n"},             ///< Help option

    {"-i", read_input_filename,  "#   -i - specify input file name after this\n"},  ///< Input filename

    {"-o", read_output_filename, "#   -o - specify output file name after this\n"}, ///< Output filename

    {"-d", enable_debug_mode,    "#   -d - enables debug mode "
                                 "(writes byte posiotion of every command)\n"},     ///< Debug mode
};

const int ARGS_DICT_LEN = sizeof(ARGS_DICT) / sizeof(ARGS_DICT[0]);                 ///< args_dict array len


int main(int argc, char* argv[]) {

    /// Parsing console arguments
    ArgsVars args_vars = {};
    args_vars.input_filename  = "../main.exec";   //< default value
    args_vars.output_filename = "../main.disasm"; //< default value
    args_vars.debug_mode      = false;            //< default value

    STATUS_CHECK_RAISE(args_parse(argc, argv, &args_vars, ARGS_DICT, ARGS_DICT_LEN));
    /// Parsing console arguments end

    /// Reading input data
    char* inp_data = nullptr;
    long size = 0;
    STATUS_CHECK_RAISE(file_open_read_close(args_vars.input_filename, &inp_data, 0, &size),
                       FREE(inp_data));
    /// Reading input data end

    /// Parsing and writing commands
    STATUS_CHECK_RAISE(disasm_parse(inp_data, size, args_vars.output_filename, args_vars.debug_mode),
                       FREE(inp_data));
    /// Parsing and writing command end

    FREE(inp_data);

    return Status::OK_EXIT;
}

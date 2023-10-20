#include "lib/utils/statuses.h"
#include "lib/utils/args_parser.h"
#include "lib/text/text_proccessor.h"
#include "assembler.h"

/**
 * @brief This array contains console options, their functions and descriptions
 */
const Argument ARGS_DICT[] = {
    {"-h", print_help,              "#   -h - prints help information\n"},             //< Help option

    {"-i", read_input_filename,     "#   -i - specify input file name after this\n"},  //< Input filename

    {"-o", read_output_filename,    "#   -o - specify output file name after this\n"}, //< Output filename

    {"-l", read_listing_filename,   "#   -l - enables and specifies listing file\n"},  //< Listing file
};

const int ARGS_DICT_LEN = sizeof(ARGS_DICT) / sizeof(ARGS_DICT[0]); //< args_dict array len


int main(int argc, char* argv[]) {

    /// Parsing console arguments
    ArgsVars args_vars = {};
    args_vars.input_filename    = "../Programs/example/main.code"; //< default value
    args_vars.output_filename   = "../Programs/example/main.exec"; //< default value
    args_vars.listing_filename  = "../Programs/example/main.list"; //< default value

    STATUS_CHECK_RAISE(args_parse(argc, argv, &args_vars, ARGS_DICT, ARGS_DICT_LEN));
    /// Parsing console arguments end

    /// Reading and parsing input file
    InputData input_data = {};
    input_data.ctor(args_vars.input_filename);
    /// Reading and parsing input file end

    /// Assembling and writing
    STATUS_CHECK_RAISE(assemeble_and_write(&input_data, args_vars.output_filename,
                                           args_vars.input_filename, args_vars.listing_filename),
                       input_data.detor(););
    /// Assembling and writing end

    input_data.detor();

    return Status::OK_EXIT;
}

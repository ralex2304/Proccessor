#include "utils/statuses.h"
#include "utils/args_parser.h"
#include "text/text_proccessor.h"
#include "assembler.h"


int main(int argc, char* argv[]) {

    /// Parsing console arguments
    ArgsVars args_vars = {};
    args_vars.input_filename  = "../main.code"; ///< default value
    args_vars.output_filename = "../main.exec"; ///< default value
    args_vars.text_mode       = false;          ///< default value

    STATUS_CHECK(args_parse(argc, argv, &args_vars));
    /// Parsing console arguments end

    /// Reading and parsing input file
    InputData input_data = {};
    input_data.ctor(args_vars.input_filename);
    /// Reading and parsing input file end

    /// Assembling and writing
    STATUS_CHECK(assmeble_and_write(&input_data, args_vars.output_filename, args_vars.text_mode));
    /// Assembling and writing end

    input_data.detor();

    return Status::OK_EXIT;
}

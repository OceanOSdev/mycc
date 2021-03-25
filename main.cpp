#include <stdio.h>
#include <vector>
#include "arg_parser.h"
#include "lexeme_data.h"
#include "logger.h"
#include "driver.h"

const char* versionInfo =
"My bare-bones C compiler (for COM 440)\n"
"\tWritten by Thomas Maloney (tmaloney@iastate.edu)\n"
"\tVersion 0.3\n"
"\t4 March, 2021\n";

Logger* logger;

void print_token_list(std::vector<LexemeDataNode> tlist) {
    std::vector<LexemeDataNode>::iterator iter;
    for( iter = tlist.begin(); iter != tlist.end(); iter++) {
        if (iter->is_error)
            logger->log_lex_err_v2(*iter);
        else
            logger->log_lex_info_v2(*iter);
    }
}

// void print_symbol_parse_list(FILE* fout, symbol_parse_list_t** spls, int numLists) {
//     for (int i = 0; i < numLists; i ++) {
//         for (int j = 0; j < spls[i]->num_glob_structs; j ++) {
//             log_parser_glob_struct_symbol(fout, spls[i]->glob_structs[j]);
//             fprintf(fout, "\n");
//         }
//         if (spls[i]->num_glob_vars > 0) {
//             fprintf(fout,"Global variables\n\t");
//             log_string_list(fout, spls[i]->global_variables);
//             fprintf(fout,"\n\n");
//         }
        
//         symbol_parse_list_node_t* iter = spls[i]->head;
//         while (iter) {
//             switch (iter->symbol_type) {
//                 case FUNC_DECL_SYMBOL:
//                     log_parser_func_decl_symbol(fout, iter->symbol_data.fds_val);
//                     fprintf(fout, "\n");
//                     break;
//                 case FUNC_PROTO_SYMBOL:
//                     log_parser_func_proto_symbol(fout, iter->symbol_data.fps_val);
//                     fprintf(fout, "\n");
//                     break;
//                 case PARSE_ERROR_SYMBOL:
//                     log_parser_error(fout, iter->symbol_data.pes_val);
//                     fprintf(fout != stdout ? fout : stderr, "\n");
//                     break;
//             }
//             iter = iter->next;
//         }
//     }
// }

void runLexer(parsed_args_t* pat) {
    Driver d;
    for (int i = 0; i < pat->numFiles; i++) {
        std::string filename = std::string( pat->inputFiles[i]);
        std::ifstream ifstrm(filename);
        std::istream* ist = &ifstrm;
        d.switch_input_stream(filename, ist);
        d.init_new_input();
        while ((d.lex()).type >= 1) {}

    }

    auto tlist = d.get_part_one_lexeme_list();
    print_token_list(tlist);
}

void runParser(parsed_args_t* pat) {
    Driver d;
    for (int i = 0; i < pat->numFiles; i++) {
        std::string filename = std::string( pat->inputFiles[i]);
        std::ifstream ifstrm(filename);
        std::istream* ist = &ifstrm;
        d.switch_input_stream(filename, ist);
        d.init_new_input();
        d.parse();
    }
    //init_stacks();
    // symbol_parse_list_t** spls = malloc(pat->numFiles * sizeof(symbol_parse_list_t*));
    // int i;
    // for (i = 0; i < pat->numFiles; i ++) {
    //     reset_stacks();
    //     spls[i] = create_symbol_parse_list();
    //     spl = spls[i];
    //     input_comp_file = pat->inputFiles[i];
    //     yyin = fopen(pat->inputFiles[i], "r");
    //     yyparse();
    //     fclose(yyin);
    // }

    // print_symbol_parse_list(fout, spls, pat->numFiles);
}

void handleArgs(parsed_args_t* pat, char* oFileName) {
    if (pat->useOutputFile) {
        logger = new Logger(std::string(oFileName));
    } else {
        logger = new Logger();
    }
    
    switch (pat->mode) {
        case MODE_ERR: break; // <-- Should literally never happen
        case MODE_ZERO: logger->log_info(std::string(versionInfo)); break;
        case MODE_ONE: runLexer(pat); break;
        case MODE_TWO: runParser(pat); break;
        case MODE_THREE: break;
        case MODE_FOUR: break;
        case MODE_FIVE: break;
    }

    // probably shouldn't close stdout
    //if (pat->useOutputFile)
        //fclose(fout);
}

int main(int argc, char* argv[]) {
//#ifdef YYDEBUG
    //yydebug = 1;
//#endif
    //head = init_token_list();
    char* oFileName = NULL;
    parsed_args_t* pat = parseArgs(argc, argv, &oFileName);
    handleArgs(pat, oFileName);
    //unalloc_token_list(head);
    return 0;
}
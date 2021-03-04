#include <stdio.h>
#include <stdlib.h>
#include "args_parser.h"
#include "token_list.h"
#include "log_utils.h"

const char* versionInfo =
"My bare-bones C compiler (for COM 440)\n"
"\tWritten by Thomas Maloney (tmaloney@iastate.edu)\n"
"\tVersion 0.3\n"
"\t3 March, 2021\n";

char** input_comp_files;
char* input_comp_file;
token_list_node_t* head;
symbol_parse_list_t* spl;
int yyparse();
int yylex();
extern int yydebug;
extern FILE *yyin;
extern int yylineno;
//char* current_filename;
extern void reset_variable_stack();
extern void init_variable_stack();
extern void reset_param_stack();
extern void init_param_stack();

void print_token_list(FILE* fout) {
    token_list_node_t* cur = head->next;
    while (cur) {
        if (cur->isError)
            log_lex_err(fout, cur->errDesc, cur->filename, cur->lineNum, cur->text);
        else
            log_lex_info(fout, cur->filename, cur->lineNum, cur->text, cur->token);
        
        cur = cur->next;
    }
}

void print_symbol_parse_list(FILE* fout, symbol_parse_list_t** spls, int numLists) {
    for (int i = 0; i < numLists; i ++) {
        if (spls[i]->num_glob_vars > 0) {
            fprintf(fout,"Global variables\n\t");
            log_string_list(fout, spls[i]->global_variables);
            fprintf(fout,"\n\n");
        }
        
        symbol_parse_list_node_t* iter = spls[i]->head;
        while (iter) {
            switch (iter->symbol_type) {
                case FUNC_DECL_SYMBOL:
                    log_parser_func_decl_symbol(fout, iter->symbol_data.fds_val);
                    fprintf(fout, "\n");
                    break;
                case FUNC_PROTO_SYMBOL:
                    log_parser_func_proto_symbol(fout, iter->symbol_data.fps_val);
                    fprintf(fout, "\n");
                    break;
                case PARSE_ERROR_SYMBOL:
                    log_parser_error(fout, iter->symbol_data.pes_val);
                    fprintf(fout != stdout ? fout : stderr, "\n");
                    break;
            }
            iter = iter->next;
        }
    }
}

void runLexer(parsed_args_t* pat, FILE* fout) {
    input_comp_files = pat->inputFiles;
    int i;
    for (i = 0; i < pat->numFiles; i ++) {
        yylineno = 1;
        input_comp_file = pat->inputFiles[i];
        yyin = fopen(pat->inputFiles[i], "r");
        while (yylex() > 1) {}
        fclose(yyin);
    }
    print_token_list(fout);
}

void runParser(parsed_args_t* pat, FILE* fout) {
    input_comp_files = pat->inputFiles;
    init_variable_stack();
    init_param_stack();
    symbol_parse_list_t** spls = malloc(pat->numFiles * sizeof(symbol_parse_list_t*));
    int i;
    for (i = 0; i < pat->numFiles; i ++) {
        reset_variable_stack();
        reset_param_stack();
        spls[i] = create_symbol_parse_list();
        spl = spls[i];
        input_comp_file = pat->inputFiles[i];
        yyin = fopen(pat->inputFiles[i], "r");
        yyparse();
        fclose(yyin);
    }

    print_symbol_parse_list(fout, spls, pat->numFiles);
}

void handleArgs(parsed_args_t* pat, char* oFileName) {
    FILE *fout = pat->useOutputFile 
                    ? fopen(oFileName, "w+")
                    : stdout; 
    
    switch (pat->mode) {
        case MODE_ERR: break; // <-- Should literally never happen
        case MODE_ZERO: fprintf(fout, versionInfo); break;
        case MODE_ONE: runLexer(pat, fout); break;
        case MODE_TWO: runParser(pat, fout); break;
        case MODE_THREE: break;
        case MODE_FOUR: break;
        case MODE_FIVE: break;
    }

    // probably shouldn't close stdout
    if (pat->useOutputFile)
        fclose(fout);
}

int main(int argc, char* argv[]) {
//#ifdef YYDEBUG
    //yydebug = 1;
//#endif
    head = init_token_list();
    char* oFileName = NULL;
    parsed_args_t* pat = parseArgs(argc, argv, &oFileName);
    handleArgs(pat, oFileName);
    //unalloc_token_list(head);
    return 0;
}
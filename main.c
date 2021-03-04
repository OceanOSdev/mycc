#include <stdio.h>
#include <stdlib.h>
#include "args_parser.h"
#include "token_list.h"
#include "log_utils.h"

const char* versionInfo =
"My bare-bones C compiler (for COM 440)\n"
"\tWritten by Thomas Maloney (tmaloney@iastate.edu)\n"
"\tVersion 0.2\n"
"\t16 February, 2021\n";

char** input_comp_files;
char* input_comp_file;
token_list_node_t* head;
int yyparse();
int yylex();
extern int yydebug;
extern FILE *yyin;
extern int yylineno;

void print_token_list() {
    token_list_node_t* cur = head->next;
    while (cur) {
        if (cur->isError)
            log_lex_err(cur->errDesc, cur->filename, cur->lineNum, cur->text);
        else
            log_lex_info(cur->filename, cur->lineNum, cur->text, cur->token);
        
        cur = cur->next;
    }
}

void runLexer(parsed_args_t* pat) {
    input_comp_files = pat->inputFiles;
    int i;
    for (i = 0; i < pat->numFiles; i ++) {
        yylineno = 1;
        input_comp_file = pat->inputFiles[i];
        yyin = fopen(pat->inputFiles[i], "r");
        //printf("File: %s\n", pat->inputFiles[i]);
        while (yylex() > 1) {}
        fclose(yyin);
    }
    print_token_list();
}

void runParser(parsed_args_t* pat) {
    input_comp_files = pat->inputFiles;
    int i;
    for (i = 0; i < pat->numFiles; i ++) {
        input_comp_file = pat->inputFiles[i];
        yyin = fopen(pat->inputFiles[i], "r");
        //printf("File: %s\n", pat->inputFiles[i]);
        yyparse();
        fclose(yyin);
    }
}

void handleArgs(parsed_args_t* pat, char* oFileName) {
    FILE *fout = pat->useOutputFile 
                    ? fopen(oFileName, "w+")
                    : stdout; 
    
    switch (pat->mode) {
        case MODE_ERR: break; // <-- Should literally never happen
        case MODE_ZERO: fprintf(fout, versionInfo); break;
        case MODE_ONE: runLexer(pat); break;
        case MODE_TWO: runParser(pat); break;
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
//    yydebug = 1;
//#endif
    head = init_token_list();
    char* oFileName = NULL;
    parsed_args_t* pat = parseArgs(argc, argv, &oFileName);
    handleArgs(pat, oFileName);
    //unalloc_token_list(head);
    return 0;
}
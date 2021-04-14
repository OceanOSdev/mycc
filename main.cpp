#include <stdio.h>
#include <vector>
#include "arg_parser.h"
#include "lexeme_data.h"
#include "logger.h"
#include "driver.h"
#include "qsem.h"
#include "syntax_tree_printer.h"
#include "part_two_syntax_check.h"

const char* versionInfo =
"My bare-bones C compiler (for COM 440)\n"
"\tWritten by Thomas Maloney (tmaloney@iastate.edu)\n"
"\tVersion 0.3\n"
"\t4 March, 2021\n";

Logger* logger;

void print_token_list(std::vector<LexemeDataNode> tlist) {
    std::vector<LexemeDataNode>::iterator iter;
    for(iter = tlist.begin(); iter != tlist.end(); iter++) {
        if (iter->is_error)
            logger->log_lex_err_v2(*iter);
        else
            logger->log_lex_info_v2(*iter);
    }
}

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

bool runParser(parsed_args_t* pat, Driver&& d) {
    for (int i = 0; i < pat->numFiles /*&& !d.error_flag()*/; i++) {
        std::string filename = std::string( pat->inputFiles[i]);
        std::ifstream ifstrm(filename);
        std::istream* ist = &ifstrm;
        d.switch_input_stream(filename, ist);
        d.init_new_input();
        d.parse();
    }

    return !d.error_flag();
}

void runSyntaxChecker(parsed_args_t* pat) {
    Driver d;
    auto parsed = runParser(pat, std::move(d));
    auto root = new Syntax::ProgramNode(d.get_translation_units());
    auto synt = new PartTwoSyntaxPrinter(root, logger);
    synt->print_info();
    if (!parsed)
        for (auto diagnostic : d.get_diagnostics())
            logger->log_err(diagnostic);
    //SyntaxTreePrinter::print_nodes(tun[0]);


    // print_symbol_parse_list(fout, spls, pat->numFiles);
}

void runSemanticAnalyzer(parsed_args_t* pat) {
    Driver d;
    if (runParser(pat, std::move(d))) {
        //auto root = new Syntax::ProgramNode(d.get_translation_units());
        SyntaxTreePrinter::print_nodes(d.get_translation_units()[0]);
        //QuickSemanticAnalyzer::analyze(root);
    } else {
        for (auto diagnostic : d.get_diagnostics())
            logger->log_err(diagnostic);
    }

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
        case MODE_TWO: runSyntaxChecker(pat); break;
        case MODE_THREE:runSemanticAnalyzer(pat); break;
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
#include <stdio.h>
#include <vector>
#include "arg_parser.h"
#include "lexeme_data.h"
#include "logging/logger.h"
#include "driver.h"
#include "qsem.h"
#include "syntax_tree_printer.h"
#include "part_two_syntax_check.h"
#include "logging/diagnostics.h"
#include "binding/binder.h"
#include "bound_tree_printer.h"

const char* version_info =
"My bare-bones C compiler (for COM 440)\n"
"\tWritten by Thomas Maloney (tmaloney@iastate.edu)\n"
"\tVersion 0.3\n"
"\t4 March, 2021\n";

Logging::Logger* logger;

void argument_handler(Arguments* args);
bool run_parser(Arguments* pat, Driver&& d);
void print_token_list(std::vector<LexemeDataNode> tlist);
void run_lexer(Arguments* args); 
void run_syntax_checker(Arguments* args); 
void run_semantic_analyzer(Arguments* args);
void run_syntax_tree_printer(Arguments* args);
void run_bound_tree_printer(Arguments* args);

int main(int argc, char* argv[]) {
    auto arguments = Arguments::parse_arguments(argc, argv);
    argument_handler(arguments);
    return 0;
}

void argument_handler(Arguments* args) {
    if (args->should_output_file()) {
        logger = new Logging::Logger(args->output_filename());
    } else {
        logger= new Logging::Logger();
    }

    switch (args->compiler_mode()) {
        case CompilerMode::MODE_ERR: break; // <-- should literally never happen.
        case CompilerMode::MODE_ZERO: logger->log_info(std::string(version_info)); break;
        case CompilerMode::MODE_ONE: run_lexer(args); break;
        case CompilerMode::MODE_TWO: run_syntax_checker(args); break;
        case CompilerMode::MODE_THREE: run_semantic_analyzer(args); break;
        case CompilerMode::MODE_FOUR: break;
        case CompilerMode::MODE_FIVE: break;
        case CompilerMode::MODE_SIX: run_syntax_tree_printer(args); break;
        case CompilerMode::MODE_SEVEN: run_bound_tree_printer(args); break;
    }
}

void print_token_list(std::vector<LexemeDataNode> tlist) {
    std::vector<LexemeDataNode>::iterator iter;
    for(iter = tlist.begin(); iter != tlist.end(); iter++) {
        if (iter->is_error)
            logger->log_lex_err_v2(*iter);
        else
            logger->log_lex_info_v2(*iter);
    }
}

void run_lexer(Arguments* args) {
    Driver d;
    for (auto filename: args->input_filenames()) {
        std::ifstream ifstrm(filename);
        std::istream* ist = &ifstrm;
        d.switch_input_stream(filename, ist);
        d.init_new_input();
        while ((d.lex()).type >= 1) {}
    }
    auto tlist = d.get_part_one_lexeme_list();
    print_token_list(tlist);
}

bool run_parser(Arguments* args, Driver&& d) {
    for (auto filename : args->input_filenames()) {
        std::ifstream ifstrm(filename);
        std::istream* ist = &ifstrm;
        d.switch_input_stream(filename, ist);
        d.init_new_input();
        d.parse();
    }

    return !d.error_flag();
}

void run_syntax_checker(Arguments* args) {
    Driver d;
    auto parsed = run_parser(args, std::move(d));
    auto root = new Syntax::ProgramNode(nullptr, d.get_translation_units());
    auto synt = new PartTwoSyntaxPrinter(root, logger);
    synt->print_info();
    if (!parsed)
        for (auto diagnostic : d.get_diagnostics())
            logger->log_err(diagnostic);
}

void run_semantic_analyzer(Arguments* args) {
    Driver d;
    if (run_parser(args, std::move(d))) {
        auto root = new Syntax::ProgramNode(nullptr, d.get_translation_units());
        auto binder = Binding::Binder::bind_program(root);
        QuickSemanticAnalyzer::log_analysis(logger, binder->part_three_info_list());
        if (binder->err_flag())
            logger->log_diagnostics_list(binder->diagnostics());
    } else {
        for (auto diagnostic : d.get_diagnostics())
            logger->log_err(diagnostic);
    }
}

void run_syntax_tree_printer(Arguments* args) {
    Driver d;
    auto parsed = run_parser(args, std::move(d));
    auto root = new Syntax::ProgramNode(nullptr, d.get_translation_units());
    SyntaxTreePrinter::print_nodes(root->units()[0]);
    if (!parsed)
        for (auto diagnostic : d.get_diagnostics())
            logger->log_err(diagnostic);
}

void run_bound_tree_printer(Arguments* args) {
    Driver d;
    if (run_parser(args, std::move(d))) {
        auto root = new Syntax::ProgramNode(nullptr, d.get_translation_units());
        auto binder = Binding::Binder::bind_program(root);
        QuickSemanticAnalyzer::log_analysis(logger, binder->part_three_info_list());
        if (binder->err_flag())
            logger->log_diagnostics_list(binder->diagnostics());
        else
            BoundTreePrinter::print_bound_tree(binder->global_decls());
    } else {
        for (auto diagnostic : d.get_diagnostics())
            logger->log_err(diagnostic);
    }
}
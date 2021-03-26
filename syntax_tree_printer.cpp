#include <vector>
#include <string>
#include <iostream>

#include "syntax/formal_parameter_node.h"
#include "syntax/operator_tokens.h"
#include "syntax/expression_node.h"
#include "syntax/cast_expression_node.h"
#include "syntax/ternary_expression_node.h"
#include "syntax/binary_expression_node.h"
#include "syntax/unary_expression_node.h"
#include "syntax/call_expression_node.h"
#include "syntax/name_expression_node.h"
#include "syntax/index_expression_node.h"
#include "syntax/member_expression_node.h"
#include "syntax/assignment_expression_node.h"
#include "syntax/decrement_expression_node.h"
#include "syntax/increment_expression_node.h"
#include "syntax/partial_variable_declaration_node.h"
#include "syntax/variable_group_declaration_node.h"
#include "syntax/literal_val_expression_node.h"
#include "syntax/statement_node.h"
#include "syntax/expression_statement_node.h"
#include "syntax/continue_statement_node.h"
#include "syntax/break_statement_node.h"
#include "syntax/return_statement_node.h"
#include "syntax/if_statement_node.h"
#include "syntax/block_statement_node.h"
#include "syntax/for_statement_node.h"
#include "syntax/while_statement_node.h"
#include "syntax/do_while_statement_node.h"
#include "syntax/struct_declaration_node.h"
#include "syntax/function_declaration_node.h"
#include "syntax/function_prototype_node.h"
#include "syntax/function_definition_node.h"
#include "syntax/global_declaration_node.h"
#include "syntax/local_declaration_node.h"
#include "syntax/global_struct_declaration_node.h"
#include "syntax/global_variable_group_declaration_node.h"
#include "syntax/translation_unit_node.h"

#include "syntax_tree_printer.h"
#include "type_checking_util.h"


namespace SyntaxTreePrinter {

    void print_node(Syntax::SyntaxNode* n, std::string indent);

    void print_formal_parameter_node(Syntax::FormalParameterNode *f, std::string indent ) {
        std::cout << "[Param]";
        if (f->is_const()) std::cout << "const ";
        if (f->is_struct()) std::cout << "struct ";
        std::cout << f->type_identifier() << " ";
        std::cout << f->param_name();
        if (f->is_array()) std::cout << "[]";
        std::cout << std::endl;
    }

    void print_function_declaration_node(Syntax::FunctionDeclarationNode* f, std::string indent) {
        //std::cout << indent;
        if (f->is_const()) std::cout << "const ";
        if (f->is_struct()) std::cout << "struct ";
        std::cout << f->type_identifier() << " ";
        std::cout << f->function_name() << " ";
        std::cout << std::endl;
        auto parameters = f->parameters();
        if (!parameters.empty()) {
            std::vector<Syntax::FormalParameterNode*>::iterator param_iter;
            auto last = std::prev(parameters.end(), 1);
            for (param_iter = parameters.begin(); param_iter != parameters.end(); param_iter++) {
                bool final = (param_iter == last);
                std::string ind = final ? "└──" : "├──"; 
                std::string mark2 = final ?  "   " : "│  ";
                auto node = *param_iter;
                std::cout << indent << ind;
                print_node(node, indent + ind);
            }
        }
    }

    void print_expression_node(Syntax::ExpressionNode* e, std::string indent) {

    }

    void print_global_declaration_node(Syntax::GlobalDeclarationNode* g, std::string indent) {
        if (auto gvgd = dynamic_cast<Syntax::GlobalVariableGroupDeclarationNode*>(g)) {
            print_node(gvgd->variable_group(), indent);

        } else if (auto gsd = dynamic_cast<Syntax::GlobalStructDeclarationNode*>(g)) {
            print_node(gsd->struct_declaration(), indent);
            
        } else if (auto fp = dynamic_cast<Syntax::FunctionPrototypeNode*>(g)) {
            print_node(fp->function_declaration(), indent);

        } else if (instanceof<Syntax::FunctionDefinitionNode>(g)) {

        }
    }

    void print_local_declaration_node(Syntax::LocalDeclarationNode* ld, std::string indent) {

    }

    void print_tranlation_unit_node(Syntax::TranslationUnitNode* t, std::string indent) {
        std::vector<Syntax::GlobalDeclarationNode*>::iterator gd_iter;
        auto gds = t->global_declarations();
        auto last = std::prev(gds.end(), 1); 
        std::cout << "[Translation Unit]" << std::endl;
        for (gd_iter = gds.begin(); gd_iter != gds.end(); gd_iter++) {
            auto node = *gd_iter;
            bool final = (gd_iter == last);
            std::string ind = final ? "└──" : "├──";
            std::string mark2 = final ?  "   " : "│  ";
            std::cout << ind;
            print_node(node, indent + mark2);
        }
    }

    void print_node(Syntax::SyntaxNode* n, std::string indent) {
        if (instanceof<Syntax::PartialVariableDeclarationNode>(n)) {

        } else if (auto fp = dynamic_cast<Syntax::FormalParameterNode*>(n)) {
            print_formal_parameter_node(fp, indent);

        } else if (auto ld = dynamic_cast<Syntax::LocalDeclarationNode*>(n)) {


        } else if (auto fd = dynamic_cast<Syntax::FunctionDeclarationNode*>(n)) {
            print_function_declaration_node(fd, indent);
            
        } else if (auto e = dynamic_cast<Syntax::ExpressionNode*>(n)) {
            print_expression_node(e, indent);

        } else if (auto s = dynamic_cast<Syntax::StatementNode*>(n)) {


        } else if (auto gd = dynamic_cast<Syntax::GlobalDeclarationNode*>(n)) {
            print_global_declaration_node(gd, indent);

        } else if (auto tu = dynamic_cast<Syntax::TranslationUnitNode*>(n)) {
            print_tranlation_unit_node(tu, indent);
        }
    }

    void print_nodes(Syntax::SyntaxNode* n) {
        print_node(n, "");
    }
}
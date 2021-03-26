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
    void print_syntax_token_type(Syntax::SyntaxTokenType token, std::string indent);

    void print_node(Syntax::SyntaxNode* n, std::string indent);

    void print_formal_parameter_node(Syntax::FormalParameterNode *f, std::string indent ) {
        std::cout << "[Param]: ";
        if (f->is_const()) std::cout << "const ";
        if (f->is_struct()) std::cout << "struct ";
        std::cout << f->type_identifier() << " ";
        std::cout << f->param_name();
        if (f->is_array()) std::cout << "[]";
        std::cout << std::endl;
    }

    void print_function_declaration_node(Syntax::FunctionDeclarationNode* f, std::string indent) {
        //std::cout << indent;
        std::cout << "[Func Decl]: ";
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
                print_node(node, indent + mark2);
            }
        }
    }

    void print_expression_node(Syntax::ExpressionNode* e, std::string indent) {
        if (auto ce = dynamic_cast<Syntax::CastExpressionNode*>(e)) {
            std::cout << "[Cast Expr]: ";
            std::cout << ce->type();
            std::cout << std::endl;
            std::cout << indent + "└──";
            auto node = ce->expression();
            print_node(node, indent + "   ");

        } else if (auto te = dynamic_cast<Syntax::TernaryExpressionNode*>(e)) {
            std::cout << "[Ternary Expr]: ";
            std::cout << std::endl;
            std::cout << indent + "├──";
            auto cond_node = te->conditional_expression();
            print_node(cond_node, indent + "│  ");
            std::cout << indent + "├──";
            auto t_node = te->true_expression();
            print_node(t_node, indent + "│  ");
            std::cout << indent + "└──";
            auto f_node = te->false_expression();
            print_node(f_node, indent + "   ");

        } else if (auto ie = dynamic_cast<Syntax::IncrementExpressionNode*>(e)) {
            if (ie->is_post()) std::cout << "[Post-Incr Expr]: ";
            else std::cout << "[Pre-Incr Expr]: ";
            std::cout << std::endl;
            std::cout << indent + "└──";
            auto node = ie->identifier_expression();
            print_node(node, indent + "   ");

        } else if (auto de = dynamic_cast<Syntax::DecrementExpressionNode*>(e)) {
            if (de->is_post()) std::cout << "[Post-Decr Expr]: ";
            else std::cout << "[Pre-Decr Expr]: ";
            std::cout << std::endl;
            std::cout << indent + "└──";
            auto node = de->identifier_expression();
            print_node(node, indent + "   ");

        } else if (auto ae = dynamic_cast<Syntax::AssignmentExpressionNode*>(e)) {
            std::cout << "[Assignment Expr]: ";
            std::cout << std::endl;

            std::cout << indent + "├──";
            auto id_node = ae->identifier_expression();
            print_node(id_node, indent + "│  ");
            std::cout << indent + "├──";
            auto a_t = ae->assignment_type();
            print_syntax_token_type(a_t, indent + "│  ");
            std::cout << indent + "└──";
            auto node = ae->expression();
            print_node(node, indent + "   ");

        } else if (auto cle = dynamic_cast<Syntax::CallExpressionNode*>(e)) {

        } else if (auto ne = dynamic_cast<Syntax::NameExpressionNode*>(e)) {
            std::cout << "[Name Expr]: ";
            std::cout << ne->name();
            std::cout << std::endl;

        } else if (auto ine = dynamic_cast<Syntax::IndexExpressionNode*>(e)) {

        } else if (auto me = dynamic_cast<Syntax::MemberExpressionNode*>(e)) {

        } else if (auto ue = dynamic_cast<Syntax::UnaryExpressionNode*>(e)) {

        } else if (auto be = dynamic_cast<Syntax::BinaryExpressionNode*>(e)) {

        } else if (auto lve = dynamic_cast<Syntax::LiteralValExpressionNode*>(e)) {
            std::cout << "[Literal Val Expr]: ";
            switch (lve->value_type()) {
                case Syntax::LiteralValType::CHARCONST:
                    std::cout << lve->char_value();
                    break;
                case Syntax::LiteralValType::STRCONST:
                    std::cout << lve->string_value();
                    break;
                case Syntax::LiteralValType::INTCONST:
                    std::cout << lve->int_value();
                    break;
                case Syntax::LiteralValType::REALCONST:
                    std::cout << lve->float_value();
                    break;
            }
            std::cout << std::endl;
        }
    }

    void print_statement_node(Syntax::StatementNode* s, std::string indent) {

    }

    void print_global_declaration_node(Syntax::GlobalDeclarationNode* g, std::string indent) {
        if (auto gvgd = dynamic_cast<Syntax::GlobalVariableGroupDeclarationNode*>(g)) {
            print_node(gvgd->variable_group(), indent);

        } else if (auto gsd = dynamic_cast<Syntax::GlobalStructDeclarationNode*>(g)) {
            print_node(gsd->struct_declaration(), indent);
            
        } else if (auto fp = dynamic_cast<Syntax::FunctionPrototypeNode*>(g)) {
            print_node(fp->function_declaration(), indent);

        } else if (auto fd = dynamic_cast<Syntax::FunctionDefinitionNode*>(g)) {

        }
    }

    void print_local_declaration_node(Syntax::LocalDeclarationNode* ld, std::string indent) {
        if (auto vgd = dynamic_cast<Syntax::VariableGroupDeclarationNode*>(ld)) {
            std::cout << "[Variable Decl Group]: ";
            if (vgd->is_const()) std::cout << "const ";
            if (vgd->is_struct()) std::cout << "struct ";
            std::cout << vgd->type();
            std::cout << std::endl;
            auto vars = vgd->partial_variable_group();
            std::vector<Syntax::PartialVariableDeclarationNode*>::iterator iter;
            auto last = std::prev(vars.end(),1);
            for (iter = vars.begin(); iter != vars.end(); iter++) {
                bool final = (iter == last);
                std::string ind = final ? "└──" : "├──"; 
                std::string mark2 = final ?  "   " : "│  ";
                auto node = *iter;
                std::cout << indent << ind;
                print_node(node, indent + mark2);
            }
        } else if (auto sd = dynamic_cast<Syntax::StructDeclarationNode*>(ld)) {
            std::cout << "[Struct Decl]: ";
            std::cout << sd->struct_name();
            std::cout << std::endl;
            auto vars = sd->struct_members();
            std::vector<Syntax::VariableGroupDeclarationNode*>::iterator iter;
            auto last = std::prev(vars.end(),1);
            for (iter = vars.begin(); iter != vars.end(); iter++) {
                bool final = (iter == last);
                std::string ind = final ? "└──" : "├──"; 
                std::string mark2 = final ?  "   " : "│  ";
                auto node = *iter;
                std::cout << indent << ind;
                print_node(node, indent + mark2);
            }
        }
    }

    void print_partial_variable_declaration_node(Syntax::PartialVariableDeclarationNode* p, std::string indent) {
        std::cout << "[Partial Var Decl]: ";
        std::cout << p->identifier();
        if (p->is_array()) {
            std::cout << "[" << p->array_length() << "] ";
        }
        std::cout << std::endl;
        if (p->is_assigned()) {
            std::cout << indent + "└──";
            auto node = p->expression();
            print_node(node, indent + "   ");
        }
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
        if (auto pvd = dynamic_cast<Syntax::PartialVariableDeclarationNode*>(n)) {
            print_partial_variable_declaration_node(pvd, indent);

        } else if (auto fp = dynamic_cast<Syntax::FormalParameterNode*>(n)) {
            print_formal_parameter_node(fp, indent);

        } else if (auto ld = dynamic_cast<Syntax::LocalDeclarationNode*>(n)) {
            print_local_declaration_node(ld, indent);

        } else if (auto fd = dynamic_cast<Syntax::FunctionDeclarationNode*>(n)) {
            print_function_declaration_node(fd, indent);
            
        } else if (auto e = dynamic_cast<Syntax::ExpressionNode*>(n)) {
            print_expression_node(e, indent);

        } else if (auto s = dynamic_cast<Syntax::StatementNode*>(n)) {
            print_statement_node(s, indent);

        } else if (auto gd = dynamic_cast<Syntax::GlobalDeclarationNode*>(n)) {
            print_global_declaration_node(gd, indent);

        } else if (auto tu = dynamic_cast<Syntax::TranslationUnitNode*>(n)) {
            print_tranlation_unit_node(tu, indent);
        }
    }

    void print_nodes(Syntax::SyntaxNode* n) {
        print_node(n, "");
    }

    void print_syntax_token_type(Syntax::SyntaxTokenType token, std::string indent) {
        switch(token) {
            case Syntax::SyntaxTokenType::PLUS:
                std::cout << indent << "PLUS" << std::endl;
                break;
            case Syntax::SyntaxTokenType::MINUS:
                std::cout << indent << "MINUS" << std::endl;
                break;
            case Syntax::SyntaxTokenType::STAR:
                std::cout << indent << "STAR" << std::endl;
                break;
            case Syntax::SyntaxTokenType::SLASH:
                std::cout << indent << "SLASH" << std::endl;
                break;
            case Syntax::SyntaxTokenType::MOD:
                std::cout << indent << "MOD" << std::endl;
                break;
            case Syntax::SyntaxTokenType::TILDE:
                std::cout << indent << "TILDE" << std::endl;
                break;
            case Syntax::SyntaxTokenType::PIPE:
                std::cout << indent << "PIPE" << std::endl;
                break;
            case Syntax::SyntaxTokenType::AMP:
                std::cout << indent << "AMP" << std::endl;
                break;
            case Syntax::SyntaxTokenType::BANG:
                std::cout << indent << "BANG" << std::endl;
                break;
            case Syntax::SyntaxTokenType::DPIPE:
                std::cout << indent << "DPIPE" << std::endl;
                break;
            case Syntax::SyntaxTokenType::DAMP:
                std::cout << indent << "DAMP" << std::endl;
                break;
            case Syntax::SyntaxTokenType::ASSIGN:
                std::cout << indent << "ASSIGN" << std::endl;
                break;
            case Syntax::SyntaxTokenType::PLUSASSIGN:
                std::cout << indent << "PLUSASSIGN" << std::endl;
                break;
            case Syntax::SyntaxTokenType::MINUSASSIGN:
                std::cout << indent << "MINUSASSIGN" << std::endl;
                break;
            case Syntax::SyntaxTokenType::STARASSIGN:
                std::cout << indent << "STARASSIGN" << std::endl;
                break;
            case Syntax::SyntaxTokenType::SLASHASSIGN:
                std::cout << indent << "SLASHASSIGN" << std::endl;
                break;
            case Syntax::SyntaxTokenType::INCR:
                std::cout << indent << "INCR" << std::endl;
                break;
            case Syntax::SyntaxTokenType::DECR:
                std::cout << indent << "DECR" << std::endl;
                break;
            case Syntax::SyntaxTokenType::EQUALS:
                std::cout << indent << "EQUALS" << std::endl;
                break;
            case Syntax::SyntaxTokenType::NEQUAL:
                std::cout << indent << "NEQUAL" << std::endl;
                break;
            case Syntax::SyntaxTokenType::GT:
                std::cout << indent << "GT" << std::endl;
                break;
            case Syntax::SyntaxTokenType::GE:
                std::cout << indent << "GE" << std::endl;
                break;
            case Syntax::SyntaxTokenType::LT:
                std::cout << indent << "LT" << std::endl;
                break;
            case Syntax::SyntaxTokenType::LE:
                std::cout << indent << "LE" << std::endl;
                break;
        }
    }
}
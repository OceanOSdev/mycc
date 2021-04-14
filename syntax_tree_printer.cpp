#include "syntax/syntax_token.h"
#include <vector>
#include <string>
#include <iostream>
#include "syntax_tree_printer.h"
#include "syntax/syntax_kind.h"
#include "syntax/formal_parameter_node.h"
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
#include "syntax/program_node.h"


namespace SyntaxTreePrinter {
    void print_syntax_token_type(Syntax::token_type_t token, std::string indent);
    std::string syntax_token_type_as_string(Syntax::token_type_t token);

    void print_node(Syntax::SyntaxNode* n, std::string indent);

    void print_formal_parameter_node(Syntax::FormalParameterNode *f) {
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
            std::cout << syntax_token_type_as_string(a_t) << std::endl;
            std::cout << indent + "└──";
            auto node = ae->expression();
            print_node(node, indent + "   ");

        } else if (auto cle = dynamic_cast<Syntax::CallExpressionNode*>(e)) {
            std::cout << "[Call Expr]:" << std::endl;
            std::cout << indent + "├──" << "[name]: " << cle->name() << std::endl;
            auto exprs = cle->expressions();
            std::vector<Syntax::ExpressionNode*>::iterator expr_iter;
            auto last = std::prev(exprs.end(), 1);
            for (expr_iter = exprs.begin(); expr_iter != exprs.end(); expr_iter++) {
                bool final = (expr_iter == last);
                std::string ind = final ? "└──" : "├──"; 
                std::string mark2 = final ?  "   " : "│  ";
                auto node = *expr_iter;
                std::cout << indent << ind;
                print_node(node, indent + mark2);
            }

        } else if (auto ne = dynamic_cast<Syntax::NameExpressionNode*>(e)) {
            std::cout << "[Name Expr]: ";
            std::cout << ne->name();
            std::cout << std::endl;

        } else if (auto ine = dynamic_cast<Syntax::IndexExpressionNode*>(e)) {
            std::cout << "[Index Expr]:";
            std::cout << std::endl << indent + "├──" << "[ident]: " << ine->name() << std::endl;
            std::cout << indent + "└──";
            auto node = ine->expression();
            print_node(node, indent + "   ");

        } else if (auto me = dynamic_cast<Syntax::MemberExpressionNode*>(e)) {
            std::cout << "[Member Expr]:";
            std::cout << std::endl << indent + "├──" << "[LHS]: ";
            auto lhs = me->encapsulator();
            std::cout << std::endl;
            std::cout << indent + "│  " + "└──";
            print_node(lhs, indent + "│  ");
            std::cout << indent + "└──";
            auto node = me->member();
            print_node(node, indent + "   ");

        } else if (auto ue = dynamic_cast<Syntax::UnaryExpressionNode*>(e)) {
            std::cout << "[Unary Expr]:";
            std::cout << std::endl << indent + "├──" << "[OP]: " << syntax_token_type_as_string(ue->syntax_token_type());
            std::cout << std::endl;
            std::cout << indent + "└──";
            auto node = ue->expression();
            print_node(node, indent + "   ");

        } else if (auto be = dynamic_cast<Syntax::BinaryExpressionNode*>(e)) {
            std::cout << "[Binary Expr]:" << std::endl;
            std::cout << indent + "├──";
            auto lnode = be->left_expression();
            print_node(lnode, indent + "│  ");
            std::cout << indent + "├──" << "[OP]: " << syntax_token_type_as_string(be->syntax_token_type());
            std::cout << std::endl;
            std::cout << indent + "└──";
            auto node = be->right_expression();
            print_node(node, indent + "   ");

        } else if (auto lve = dynamic_cast<Syntax::LiteralValExpressionNode*>(e)) {
            std::cout << "[Literal Val Expr]: ";
            try {
            switch (lve->value_type()) {
                case Syntax::token_data_type::CHAR:
                    std::cout << lve->char_value();
                    break;
                case Syntax::token_data_type::STRING:
                    std::cout << lve->string_value();
                    break;
                case Syntax::token_data_type::INT:
                    std::cout << lve->int_value();
                    break;
                case Syntax::token_data_type::FLOAT:
                    std::cout << lve->float_value();
                    break;
            }
            } catch (const std::bad_variant_access& bva) {
                std::cout << bva.what();
            }
            std::cout << std::endl;
        }
    }

    void print_block_statement_node(Syntax::BlockStatementNode* node, std::string indent) {
        std::cout << "[Block]:" << std::endl;
        auto stmts = node->statements();
        std::vector<Syntax::StatementNode*>::iterator iter;
        auto last = std::prev(stmts.end(),1);
        for (iter = stmts.begin(); iter != stmts.end(); iter++) {
            bool final = (iter == last);
            std::string ind = final ? "└──" : "├──"; 
            std::string mark2 = final ?  "   " : "│  ";
            auto fnode = *iter;
            std::cout << indent << ind;
            print_node(fnode, indent + mark2);
        }
    }

    void print_break_statement_node() {
        std::cout << "[BREAK]" << std::endl;
    }

    void print_continue_statement_node() {
        std::cout << "[CONTINUE]" << std::endl;
    }

    void print_do_while_statement_node(Syntax::DoWhileStatementNode* node, std::string indent) {
        std::cout << "[DOWHILE]:" << std::endl;
        std::cout << indent + "├──" << "[BODY]:" << std::endl;
        print_node(node->body_statement(), indent + "│  ");
        std::cout << indent + "└──" << "[COND]:" << std::endl;
        print_node(node->conditional_expression(), indent + "   ");
    }

    void print_expression_statement_node(Syntax::ExpressionStatementNode* node, std::string indent) {
        print_node(node->expression(), indent);
    }

    void print_for_statement_node(__attribute__((unused)) Syntax::ForStatementNode* node, 
                                  __attribute__((unused)) std::string indent) {

    }

    void print_if_statement_node(Syntax::IfStatementNode* node, std::string indent) {
        std::cout << "[IF]:" << std::endl;
        std::cout << indent + "├──" << "[COND]:" << std::endl;
        print_node(node->condition(), indent + "│  ");
        std::string marker = "├──";
        std::string mind = "│  ";
        if (!node->has_else_statement()) {
            marker = "└──";
            mind = "   ";
        }
        std::cout << indent + marker << "[THEN]:" << std::endl;
        print_node(node->then_statement(), indent + mind);

        if (node->has_else_statement()) {
            std::cout << indent + "└──" << "[ELSE]:" << std::endl;
            print_node(node->then_statement(), indent + "   ");
        }
    }

    void print_return_statement_node(Syntax::ReturnStatementNode* node, std::string indent) {
        std::cout << "[RETURN]:" << std::endl;
        if (!node->is_empty_return()) {
            std::cout << indent + "└──";
            print_node(node->expression(), indent + "   ");
        }
    }

    void print_while_statement_node(Syntax::WhileStatementNode* node, std::string indent) {
        std::cout << "[WHILE]:" << std::endl;
        std::cout << indent + "├──" << "[COND]:" << std::endl;
        std::string tind = indent + "│  ";
        std::cout << tind + "└──";
        print_node(node->conditional_expression(), tind  + "   ");
        std::cout << indent + "└──" << "[BODY]:" << std::endl;
        print_node(node->body_statement(), indent + "   ");
    }


    void print_statement_node(Syntax::StatementNode* s, std::string indent) {
        if (auto blk = dynamic_cast<Syntax::BlockStatementNode*>(s)) {
            print_block_statement_node(blk, indent);

        } else if (dynamic_cast<Syntax::BreakStatementNode*>(s)) {
            print_break_statement_node();

        } else if (dynamic_cast<Syntax::ContinueStatementNode*>(s)) {
            print_continue_statement_node();

        } else if (auto dow = dynamic_cast<Syntax::DoWhileStatementNode*>(s)) {
            print_do_while_statement_node(dow, indent);

        } else if (auto exprst = dynamic_cast<Syntax::ExpressionStatementNode*>(s)) {
            print_expression_statement_node(exprst, indent);

        } else if (auto forst = dynamic_cast<Syntax::ForStatementNode*>(s)) {
            print_for_statement_node(forst, indent);

        } else if (auto ifst = dynamic_cast<Syntax::IfStatementNode*>(s)) {
            print_if_statement_node(ifst, indent);

        } else if (auto retst = dynamic_cast<Syntax::ReturnStatementNode*>(s)) {
            print_return_statement_node(retst, indent);

        } else if (auto whilest = dynamic_cast<Syntax::WhileStatementNode*>(s)) {
            print_while_statement_node(whilest, indent);

        }
    }

    void print_global_declaration_node(Syntax::GlobalDeclarationNode* g, std::string indent) {
        if (auto gvgd = dynamic_cast<Syntax::GlobalVariableGroupDeclarationNode*>(g)) {
            print_node(gvgd->variable_group(), indent);

        } else if (auto gsd = dynamic_cast<Syntax::GlobalStructDeclarationNode*>(g)) {
            print_node(gsd->struct_declaration(), indent);
            
        } else if (auto fp = dynamic_cast<Syntax::FunctionPrototypeNode*>(g)) {
            print_node(fp->function_declaration(), indent);

        } else if (auto fd = dynamic_cast<Syntax::FunctionDefinitionNode*>(g)) {
            std::cout << "[FUNC DEF]:" << std::endl;
            
            std::cout << indent + "├──" << "[LOCALDECS]:" << std::endl;
            auto local_decs = fd->local_declarations();
            std::string tind = indent +"│  ";
            std::vector<Syntax::LocalDeclarationNode*>::iterator iter;
            auto last = std::prev(local_decs.end(),1);
            for (iter = local_decs.begin(); iter != local_decs.end(); iter++) {
                bool final = (iter == last);
                std::string ind = final ? "└──" : "├──"; 
                std::string mark2 = final ?  "   " : "│  ";
                auto fnode = *iter;
                std::cout << tind << ind;
                print_node(fnode, tind + mark2);
            }

            
            std::cout << indent + "└──" << "[STMTS]:" << std::endl;
            std::string sind = indent + "   ";
            auto stmts = fd->statements();
            std::vector<Syntax::StatementNode*>::iterator stmtiter;
            auto laststmt = std::prev(stmts.end(),1);
            for (stmtiter = stmts.begin(); stmtiter != stmts.end(); stmtiter++) {
                bool final = (stmtiter == laststmt);
                std::string ind = final ? "└──" : "├──"; 
                std::string mark2 = final ?  "   " : "│  ";
                auto fnode = *stmtiter;
                std::cout << sind << ind;
                print_node(fnode, sind + mark2);
            }
            //std::cout << indent + "├──";
            //print_node(fd->function_declaration(), indent + "│  ");
            //std::cout << indent + "├──";
            //print_node(fd->local_declarations(), indent + "│  ");
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
            print_formal_parameter_node(fp);

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

    void print_syntax_token_type(Syntax::token_type_t token, std::string indent) {
        switch(token) {
            case Syntax::token_type_t::PLUS:
                std::cout << indent << "PLUS" << std::endl;
                break;
            case Syntax::token_type_t::MINUS:
                std::cout << indent << "MINUS" << std::endl;
                break;
            case Syntax::token_type_t::STAR:
                std::cout << indent << "STAR" << std::endl;
                break;
            case Syntax::token_type_t::SLASH:
                std::cout << indent << "SLASH" << std::endl;
                break;
            case Syntax::token_type_t::MOD:
                std::cout << indent << "MOD" << std::endl;
                break;
            case Syntax::token_type_t::TILDE:
                std::cout << indent << "TILDE" << std::endl;
                break;
            case Syntax::token_type_t::PIPE:
                std::cout << indent << "PIPE" << std::endl;
                break;
            case Syntax::token_type_t::AMP:
                std::cout << indent << "AMP" << std::endl;
                break;
            case Syntax::token_type_t::BANG:
                std::cout << indent << "BANG" << std::endl;
                break;
            case Syntax::token_type_t::DPIPE:
                std::cout << indent << "DPIPE" << std::endl;
                break;
            case Syntax::token_type_t::DAMP:
                std::cout << indent << "DAMP" << std::endl;
                break;
            case Syntax::token_type_t::ASSIGN:
                std::cout << indent << "ASSIGN" << std::endl;
                break;
            case Syntax::token_type_t::PLUSASSIGN:
                std::cout << indent << "PLUSASSIGN" << std::endl;
                break;
            case Syntax::token_type_t::MINUSASSIGN:
                std::cout << indent << "MINUSASSIGN" << std::endl;
                break;
            case Syntax::token_type_t::STARASSIGN:
                std::cout << indent << "STARASSIGN" << std::endl;
                break;
            case Syntax::token_type_t::SLASHASSIGN:
                std::cout << indent << "SLASHASSIGN" << std::endl;
                break;
            case Syntax::token_type_t::INCR:
                std::cout << indent << "INCR" << std::endl;
                break;
            case Syntax::token_type_t::DECR:
                std::cout << indent << "DECR" << std::endl;
                break;
            case Syntax::token_type_t::EQUALS:
                std::cout << indent << "EQUALS" << std::endl;
                break;
            case Syntax::token_type_t::NEQUAL:
                std::cout << indent << "NEQUAL" << std::endl;
                break;
            case Syntax::token_type_t::GT:
                std::cout << indent << "GT" << std::endl;
                break;
            case Syntax::token_type_t::GE:
                std::cout << indent << "GE" << std::endl;
                break;
            case Syntax::token_type_t::LT:
                std::cout << indent << "LT" << std::endl;
                break;
            case Syntax::token_type_t::LE:
                std::cout << indent << "LE" << std::endl;
                break;
            default:
                break;
        }
    }

    std::string syntax_token_type_as_string(Syntax::token_type_t token) {
        switch(token) {
            case Syntax::token_type_t::PLUS:
                return "PLUS";
                break;
            case Syntax::token_type_t::MINUS:
                return "MINUS";
                break;
            case Syntax::token_type_t::STAR:
                return "STAR";
                break;
            case Syntax::token_type_t::SLASH:
                return "SLASH";
                break;
            case Syntax::token_type_t::MOD:
                return "MOD";
                break;
            case Syntax::token_type_t::TILDE:
                return "TILDE";
                break;
            case Syntax::token_type_t::PIPE:
                return "PIPE";
                break;
            case Syntax::token_type_t::AMP:
                return "AMP";
                break;
            case Syntax::token_type_t::BANG:
                return "BANG";
                break;
            case Syntax::token_type_t::DPIPE:
                return "DPIPE";
                break;
            case Syntax::token_type_t::DAMP:
                return "DAMP";
                break;
            case Syntax::token_type_t::ASSIGN:
                return "ASSIGN";
                break;
            case Syntax::token_type_t::PLUSASSIGN:
                return "PLUSASSIGN";
                break;
            case Syntax::token_type_t::MINUSASSIGN:
                return "MINUSASSIGN";
                break;
            case Syntax::token_type_t::STARASSIGN:
                return "STARASSIGN";
                break;
            case Syntax::token_type_t::SLASHASSIGN:
                return "SLASHASSIGN";
                break;
            case Syntax::token_type_t::INCR:
                return "INCR";
                break;
            case Syntax::token_type_t::DECR:
                return "DECR";
                break;
            case Syntax::token_type_t::EQUALS:
                return "EQUALS";
                break;
            case Syntax::token_type_t::NEQUAL:
                return "NEQUAL";
                break;
            case Syntax::token_type_t::GT:
                return "GT";
                break;
            case Syntax::token_type_t::GE:
                return "GE";
                break;
            case Syntax::token_type_t::LT:
                return "LT";
                break;
            case Syntax::token_type_t::LE:
                return "LE";
                break;
            default:
                return "ERROR";
        }
    }
}
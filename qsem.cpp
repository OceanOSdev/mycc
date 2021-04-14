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
#include "syntax/syntax_token.h"
#include <vector>
#include <string>
#include <iostream>
#include <compare>
#include "qsem.h"
#include "symbols/parameter_symbol.h"
#include "symbols/type_symbol.h"
#include "symbols/function_symbol.h"
#include "symbols/struct_symbol.h"

namespace QuickSemanticAnalyzer {
    void analyze_node(Syntax::SyntaxNode* n);

    // void analyze_formal_parameter_node(Syntax::FormalParameterNode *f ) {
    //     std::cout << "Parameter: " <<  f->param_name() << " has type " << f->type_identifier() << std::endl;
    // }

    void analyze_function_declaration_node(Syntax::FunctionDeclarationNode* f) {
        auto fsymb = f->function_symbol();
        std::cout << "Function " << fsymb->name();
        //bool is_void = std::is_neq(*(fsymb->type()) <=> Symbols::TypeSymbol::Void);
        std::cout << ", returns ";
        if (fsymb->type()->attributes().is_struct)
            std::cout << "struct ";
        std::cout << fsymb->type()->name();
        std::cout << std::endl;
        std::cout << "\tParameters" << std::endl;
        for (auto param : fsymb->params()) {
            std::cout << "\t\t";
            if (param->is_constant())
                std::cout << "const ";
            if (param->type()->attributes().is_struct)
                std::cout << "struct ";
            std::cout << param->type()->name();
            if (param->is_array())
                std::cout << "[]";
            std::cout << " " << param->name();
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void analyze_expression_node(Syntax::ExpressionNode* e) {
        if (auto ce = dynamic_cast<Syntax::CastExpressionNode*>(e)) {
            auto node = ce->expression();
            analyze_node(node);

        } else if (auto te = dynamic_cast<Syntax::TernaryExpressionNode*>(e)) {
            auto cond_node = te->conditional_expression();
            analyze_node(cond_node);
            auto t_node = te->true_expression();
            analyze_node(t_node);
            auto f_node = te->false_expression();
            analyze_node(f_node);

        } else if (auto ie = dynamic_cast<Syntax::IncrementExpressionNode*>(e)) {
            auto node = ie->identifier_expression();
            analyze_node(node);

        } else if (auto de = dynamic_cast<Syntax::DecrementExpressionNode*>(e)) {
            auto node = de->identifier_expression();
            analyze_node(node);

        } else if (auto ae = dynamic_cast<Syntax::AssignmentExpressionNode*>(e)) {
            auto id_node = ae->identifier_expression();
            analyze_node(id_node);
            __attribute__((unused)) auto a_t = ae->assignment_type();
            auto node = ae->expression();
            analyze_node(node);

        } else if (auto cle = dynamic_cast<Syntax::CallExpressionNode*>(e)) {
            auto exprs = cle->expressions();
            for (auto pexpr : exprs) {
                analyze_node(pexpr);
            }

        } else if ( __attribute__((unused)) auto ne = dynamic_cast<Syntax::NameExpressionNode*>(e)) {


        } else if (auto ine = dynamic_cast<Syntax::IndexExpressionNode*>(e)) {
            auto node = ine->expression();
            analyze_node(node);

        } else if (auto me = dynamic_cast<Syntax::MemberExpressionNode*>(e)) {
            auto lhs = me->encapsulator();
            analyze_node(lhs);
            auto node = me->member();
            analyze_node(node);

        } else if (auto ue = dynamic_cast<Syntax::UnaryExpressionNode*>(e)) {
            auto node = ue->expression();
            analyze_node(node);

        } else if (auto be = dynamic_cast<Syntax::BinaryExpressionNode*>(e)) {
            auto lnode = be->left_expression();
            analyze_node(lnode);
            auto node = be->right_expression();
            analyze_node(node);

        } else if ( __attribute__((unused)) auto lve = dynamic_cast<Syntax::LiteralValExpressionNode*>(e)) {
            //std::cout << "[Literal Val Expr]: ";
            try {
            switch (lve->value_type()) {
                case Syntax::token_data_type::CHAR:
                    // std::cout << lve->char_value();
                    break;
                case Syntax::token_data_type::STRING:
                    // std::cout << lve->string_value();
                    break;
                case Syntax::token_data_type::INT:
                    // std::cout << lve->int_value();
                    break;
                case Syntax::token_data_type::FLOAT:
                    // std::cout << lve->float_value();
                    break;
            }
            } catch (const std::bad_variant_access& bva) {
                std::cout << bva.what();
            }
            //std::cout << std::endl;
        }
    }

    void analyze_block_statement_node(Syntax::BlockStatementNode* node) {
        auto stmts = node->statements();
        for (auto stmt : stmts) {
            analyze_node(stmt);
        }
    }

    void analyze_break_statement_node(__attribute__((unused)) Syntax::BreakStatementNode* node) {
        std::cout << "[BREAK]" << std::endl;
    }

    void analyze_continue_statement_node(__attribute__((unused)) Syntax::ContinueStatementNode* node) {
    }

    void analyze_do_while_statement_node(Syntax::DoWhileStatementNode* node) {
        analyze_node(node->body_statement());
        analyze_node(node->conditional_expression());
    }

    void analyze_expression_statement_node(Syntax::ExpressionStatementNode* node) {
        analyze_node(node->expression());
    }

    void analyze_for_statement_node(__attribute__((unused)) Syntax::ForStatementNode* node) {

    }

    void analyze_if_statement_node(Syntax::IfStatementNode* node) {
        analyze_node(node->condition());
        analyze_node(node->then_statement());
        if (node->has_else_statement()) {
            analyze_node(node->then_statement());
        }
    }

    void analyze_return_statement_node(Syntax::ReturnStatementNode* node) {
        if (!node->is_empty_return()) {
            analyze_node(node->expression());
        }
    }

    void analyze_while_statement_node(Syntax::WhileStatementNode* node) {
        analyze_node(node->conditional_expression());
        analyze_node(node->body_statement());
    }


    void analyze_statement_node(Syntax::StatementNode* s) {
        if (auto blk = dynamic_cast<Syntax::BlockStatementNode*>(s)) {
            analyze_block_statement_node(blk);

        } else if (auto brk = dynamic_cast<Syntax::BreakStatementNode*>(s)) {
            analyze_break_statement_node(brk);

        } else if (auto cont = dynamic_cast<Syntax::ContinueStatementNode*>(s)) {
            analyze_continue_statement_node(cont);

        } else if (auto dow = dynamic_cast<Syntax::DoWhileStatementNode*>(s)) {
            analyze_do_while_statement_node(dow);

        } else if (auto exprst = dynamic_cast<Syntax::ExpressionStatementNode*>(s)) {
            analyze_expression_statement_node(exprst);

        } else if (auto forst = dynamic_cast<Syntax::ForStatementNode*>(s)) {
            analyze_for_statement_node(forst);

        } else if (auto ifst = dynamic_cast<Syntax::IfStatementNode*>(s)) {
            analyze_if_statement_node(ifst);

        } else if (auto retst = dynamic_cast<Syntax::ReturnStatementNode*>(s)) {
            analyze_return_statement_node(retst);

        } else if (auto whilest = dynamic_cast<Syntax::WhileStatementNode*>(s)) {
            analyze_while_statement_node(whilest);

        }
    }

    void analyze_global_declaration_node(Syntax::GlobalDeclarationNode* g) {
        if (auto gvgd = dynamic_cast<Syntax::GlobalVariableGroupDeclarationNode*>(g)) {
            analyze_node(gvgd->variable_group());

        } else if (auto gsd = dynamic_cast<Syntax::GlobalStructDeclarationNode*>(g)) {
            analyze_node(gsd->struct_declaration());
            
        } else if (auto fp = dynamic_cast<Syntax::FunctionPrototypeNode*>(g)) {
            analyze_node(fp->function_declaration());

        } else if (auto fd = dynamic_cast<Syntax::FunctionDefinitionNode*>(g)) {
            analyze_node(fd->function_declaration());
            auto local_decs = fd->local_declarations();
            for (auto dec : local_decs) {
                analyze_node(dec);
            }
            
            auto stmts = fd->statements();
            for (auto stmt : stmts) {
                analyze_node(stmt);
            }
        }
    }

    void analyze_local_declaration_node(Syntax::LocalDeclarationNode* ld) {
        if (auto vgd = dynamic_cast<Syntax::VariableGroupDeclarationNode*>(ld)) {
            auto vars = vgd->partial_variable_group();
            for (auto var : vars) {
                analyze_node(var);
            }

        } else if (auto sd = dynamic_cast<Syntax::StructDeclarationNode*>(ld)) {
            auto vars = sd->struct_members();
            for (auto var : vars) {
                analyze_node(var);
            }
            
        }
    }

    void analyze_partial_variable_declaration_node(Syntax::PartialVariableDeclarationNode* p) {
        if (p->is_assigned()) {
            auto node = p->expression();
            analyze_node(node);
        }
    }

    void analyze_tranlation_unit_node(Syntax::TranslationUnitNode* t) {
        auto gds = t->global_declarations();
        for (auto gd : gds) {
            analyze_node(gd);
        }
    }

    void analyze_program_node(Syntax::ProgramNode* p) {
        auto units = p->units();
        for (auto unit : units) {
            analyze_node(unit);
        }
    }

    void analyze_node(Syntax::SyntaxNode* n) {
        if (auto pvd = dynamic_cast<Syntax::PartialVariableDeclarationNode*>(n)) {
            analyze_partial_variable_declaration_node(pvd);

        // } else if (auto fp = dynamic_cast<Syntax::FormalParameterNode*>(n)) {
        //     analyze_formal_parameter_node(fp);

        } else if (auto ld = dynamic_cast<Syntax::LocalDeclarationNode*>(n)) {
            analyze_local_declaration_node(ld);

        } else if (auto fd = dynamic_cast<Syntax::FunctionDeclarationNode*>(n)) {
            analyze_function_declaration_node(fd);
            
        } else if (auto e = dynamic_cast<Syntax::ExpressionNode*>(n)) {
            analyze_expression_node(e);

        } else if (auto s = dynamic_cast<Syntax::StatementNode*>(n)) {
            analyze_statement_node(s);

        } else if (auto gd = dynamic_cast<Syntax::GlobalDeclarationNode*>(n)) {
            analyze_global_declaration_node(gd);

        } else if (auto tu = dynamic_cast<Syntax::TranslationUnitNode*>(n)) {
            analyze_tranlation_unit_node(tu);

        } else if (auto p = dynamic_cast<Syntax::ProgramNode*>(n)) {
            analyze_program_node(p);
        }
    }

    void analyze(Syntax::SyntaxNode* n) {
        analyze_node(n);

        // auto wider = Symbols::TypeSymbol::get_wider_type(&Symbols::TypeSymbol::Char, &Symbols::TypeSymbol::Int);
        // std::cout << wider->name() << std::endl;
    }
}
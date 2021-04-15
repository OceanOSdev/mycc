#pragma once
#ifndef SYNTAX_EXPRESSION_NODE_H
#define SYNTAX_EXPRESSION_NODE_H

#include "syntax_node.h"

namespace Symbols {
    class TypeSymbol;
}

namespace Syntax {
class SyntaxToken;
class ExpressionNode : public SyntaxNode {
protected:
    const Symbols::TypeSymbol* m_expr_type;
public:
    ExpressionNode(SyntaxToken* token);

    /*
     * This method allows us to bind type information on 
     * expression nodes after/while the syntax tree is
     * being built.
     * 
     * Note: Kinda hacky and I'm not super proud of it.
     */
    void mutate_type(const Symbols::TypeSymbol* expr_type);

    /*
     * The "bound" type of this expression, initally nullptr
     */
    const Symbols::TypeSymbol* expr_type() const;
};

}

#endif
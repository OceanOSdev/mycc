#ifndef SYNTAX_PARTIAL_VARIABLE_DECLARATION_NODE_H
#define SYNTAX_PARTIAL_VARIABLE_DECLARATION_NODE_H

#include <string>
#include "syntax_node.h"
#include "expression_node.h"

namespace Syntax {

class PartialVariableDeclarationNode : public SyntaxNode {
private:
    std::string ident;
    bool is_assign;
    bool is_arr;
    int array_size;
    ExpressionNode* expr;
public:
    PartialVariableDeclarationNode(std::string id, bool assign = false, bool arr = false, int arr_c = -1, ExpressionNode* e = nullptr) :
        ident(id), is_assign(assign), is_arr(arr), array_size(arr_c), expr(e) {}
    
    ~PartialVariableDeclarationNode() { 
        if (expr != nullptr) delete expr; 
    }

    /*
     * The identifier for the partial variable declaration.
     */
    std::string identifier() const { return ident; }

    /*
     * Whether or not this variable is being assigned
     * to at declaration.
     */
    bool is_assigned() const { return is_assign; }

    /*
     * Whether or not this variable is an array.
     */
    bool is_array() const { return is_arr; }

    /*
     * The size allocated to the array variable
     * (if this variable is an array).
     * 
     * Returns -1 if is_array() returns false.
     */
    int array_length() const { return array_size; }

    /*
     * The expression that this variable gets assigned
     * (if this variable gets assigned at declaration).
     * 
     * Returns nullptr if is_assigned returns false.
     */
    ExpressionNode* expression() const { return expr; }

};

}

#endif
#include "partial_variable_declaration_node.h"
#include "syntax_kind.h"

namespace Syntax {

PartialVariableDeclarationNode::PartialVariableDeclarationNode(SyntaxToken* token, std::string id, bool assign, bool arr, int arr_c, ExpressionNode* e) : 
    SyntaxNode(token), ident(id), is_assign(assign), is_arr(arr), array_size(arr_c), expr(e) {}

PartialVariableDeclarationNode::~PartialVariableDeclarationNode() {
    if (expr != nullptr) delete expr; 
}

/*
 * The identifier for the partial variable declarati
 */
std::string PartialVariableDeclarationNode::identifier() const { 
    return ident; 
}

/*
 * Whether or not this variable is being assigned
 * to at declaration.
 */
bool PartialVariableDeclarationNode::is_assigned() const { 
    return is_assign; 
}

/*
 * Whether or not this variable is an array.
 */
bool PartialVariableDeclarationNode::is_array() const { 
    return is_arr; 
}

/*
 * The size allocated to the array variable
 * (if this variable is an array).
 * 
 * Returns -1 if is_array() returns false.
 */
int PartialVariableDeclarationNode::array_length() const { 
    return array_size; 
}

/*
 * The expression that this variable gets assigned
 * (if this variable gets assigned at declaration).
 * 
 * Returns nullptr if is_assigned returns false.
 */
ExpressionNode* PartialVariableDeclarationNode::expression() const { 
    return expr; 
}

SyntaxKind PartialVariableDeclarationNode::kind() const {
    return SyntaxKind::PartialVariableDeclaration;
}

}
#include "function_declaration_node.h"
#include "../symbols/type_symbol.h"
#include "../symbols/function_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "syntax_kind.h"

namespace Syntax {

FunctionDeclarationNode::FunctionDeclarationNode(std::string type_identifier,
                        std::string function_name,
                        std::vector<FormalParameterNode*> params,
                        bool is_struct,
                        bool is_const) :
    m_type_identifier(type_identifier),
    m_function_name(function_name),
    m_parameters(params),
    m_is_struct(is_struct),
    m_is_const(is_const) {

        Symbols::TypeAttribute t;
        t.is_struct = true;
        t.is_integer_type = false;
        t.is_numeric_type = false;

        m_type = is_struct ?
            new Symbols::TypeSymbol(type_identifier, t)
            : Symbols::TypeSymbol::try_lookup_primitive_function(type_identifier);
        
        for (auto param : params) {
            const Symbols::TypeSymbol* ptype = param->is_struct() ?
                new Symbols::TypeSymbol(param->type_identifier(), t)
                : Symbols::TypeSymbol::try_lookup_primitive(param->type_identifier());
            
            m_param_symbols.push_back(new Symbols::ParameterSymbol(param->param_name(), ptype, param->is_array(), param->is_const()));
        }

        m_function_symbol = new Symbols::FunctionSymbol(function_name, m_type, m_param_symbols);
}

FunctionDeclarationNode::~FunctionDeclarationNode() {
    std::vector<FormalParameterNode*>::iterator param_iter;
    for (param_iter = m_parameters.begin(); param_iter != m_parameters.end(); param_iter++)
        delete *param_iter;
}

/*
 * The return type for this function.
 */
std::string FunctionDeclarationNode::type_identifier() const {
    return m_type_identifier; 
}

/*
 * The return type for this function (in a more useful type).
 */
const Symbols::TypeSymbol* FunctionDeclarationNode::type() const {
    return m_type;
}

/*
 * The name of this function.
 */
std::string FunctionDeclarationNode::function_name() const {
    return m_function_name; 
}
    
/*
 * The list of parameters for this function.
 */
std::vector<FormalParameterNode*> FunctionDeclarationNode::parameters() const {
    return m_parameters; 
}

/*
 * The list of parameter symbols for this function.
 */
std::vector<Symbols::ParameterSymbol*> FunctionDeclarationNode::parameter_symbols() const {
    return m_param_symbols;
}
    
/*
 * The function symbol for this function.
 */
Symbols::FunctionSymbol* FunctionDeclarationNode::function_symbol() const {
    return m_function_symbol;
}

/*
 * Whether the function's return type is a struct type or not.
 */
bool FunctionDeclarationNode::is_struct() const {
    return m_is_struct; 
}
    
/*
 * Whether the function's return type is a constant type or not.
 */
bool FunctionDeclarationNode::is_const() const {
    return m_is_const; 
}

SyntaxKind FunctionDeclarationNode::kind() const {
    return SyntaxKind::FunctionDeclaration;
}

}
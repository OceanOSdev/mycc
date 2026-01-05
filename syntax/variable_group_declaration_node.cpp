#include "variable_group_declaration_node.h"
#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"
#include "syntax_kind.h"

namespace Syntax {

VariableGroupDeclarationNode::VariableGroupDeclarationNode(
    SyntaxToken *token, std::string type,
    std::vector<PartialVariableDeclarationNode *> vars, bool r_o, bool cust)
    : LocalDeclarationNode(token), group_type(type), dec_group(vars),
      read_only(r_o), custom_type(cust) {
  Symbols::TypeAttribute t;
  t.is_struct = true;
  t.is_integer_type = false;
  t.is_numeric_type = false;

  const Symbols::TypeSymbol *typeSymbol =
      cust ? new Symbols::TypeSymbol(type, t)
           : Symbols::TypeSymbol::try_lookup_primitive(type);

  for (auto pvd : vars) {
    std::string id = pvd->identifier();
    bool is_arr = pvd->is_array();
    int array_size = is_arr ? pvd->array_length() : -1;

    m_vars.push_back(
        new Symbols::VariableSymbol(id, typeSymbol, is_arr, array_size, r_o));
  }
}

VariableGroupDeclarationNode::~VariableGroupDeclarationNode() {
  std::vector<PartialVariableDeclarationNode *>::iterator dec_iter;
  for (dec_iter = dec_group.begin(); dec_iter != dec_group.end(); dec_iter++)
    delete *dec_iter;
}

/*
 * The type of all variables in this collection.
 */
std::string VariableGroupDeclarationNode::type() const { return group_type; }

/*
 * The list of all partial variable declarations in this group.
 */
std::vector<PartialVariableDeclarationNode *>
VariableGroupDeclarationNode::partial_variable_group() const {
  return dec_group;
}

/*
 * The list of all variables in this group (in a more useful type).
 */
std::vector<Symbols::VariableSymbol *>
VariableGroupDeclarationNode::variable_list() const {
  return m_vars;
}

/*
 * Whether or not all the variables are constant.
 */
bool VariableGroupDeclarationNode::is_const() const { return read_only; }

/*
 * Whether or not the variables are struct types.
 */
bool VariableGroupDeclarationNode::is_struct() const { return custom_type; }

SyntaxKind VariableGroupDeclarationNode::kind() const {
  return SyntaxKind::VariableDeclaration;
}

} // namespace Syntax
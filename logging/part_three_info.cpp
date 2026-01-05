#include "part_three_info.h"

namespace Logging {

void PartThreeInfoList::add_var_info(std::string _type, std::string _name,
                                     bool init, bool global) {
  if (ignore)
    return;
  PartThreeVariableInfo *var_info =
      new PartThreeVariableInfo(_type, _name, init);
  if (global) {
    global_var_info.push_back(var_info);
  } else {
    m_func_builder->local_var_info.push_back(var_info);
  }
}

void PartThreeInfoList::struct_builder_add_name(std::string name) {
  m_struct_builder->struct_name = name;
}

void PartThreeInfoList::struct_builder_add_member(std::string _type,
                                                  std::string _name) {
  PartThreeVariableInfo *var_info =
      new PartThreeVariableInfo(_type, _name, false);
  m_struct_builder->struct_members.push_back(var_info);
}

void PartThreeInfoList::struct_builder_build(bool global, bool eject) {
  if (eject) {
    reset_struct_builder();
    return;
  }

  if (global)
    global_struct_info.push_back(m_struct_builder);
  else
    m_func_builder->local_struct_info.push_back(m_struct_builder);
  reset_struct_builder();
}

void PartThreeInfoList::func_builder_add_name(std::string name) {
  if (ignore)
    return;
  m_func_builder->func_name = name;
}

void PartThreeInfoList::func_builder_add_type(std::string type) {
  if (ignore)
    return;
  m_func_builder->func_type = type;
}

void PartThreeInfoList::func_builder_add_param(std::string _type,
                                               std::string _name) {
  if (ignore)
    return;
  PartThreeVariableInfo *var_info =
      new PartThreeVariableInfo(_type, _name, false);
  m_func_builder->param_info.push_back(var_info);
}

void PartThreeInfoList::func_builder_add_statement(int lineno,
                                                   std::string stmt_type) {
  if (ignore)
    return;
  PartThreeStatementInfo *stmt = new PartThreeStatementInfo(lineno, stmt_type);
  m_func_builder->stmt_info.push_back(stmt);
}

void PartThreeInfoList::func_builder_build() {
  if (ignore)
    return;
  func_info.push_back(m_func_builder);
  reset_func_builder();
}

} // namespace Logging
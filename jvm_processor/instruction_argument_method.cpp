#include "instruction_arguments.h"
#include "../symbols/function_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "../symbols/type_symbol.h"

namespace JVMProcessor {

MethodCallInstructionArgument::MethodCallInstructionArgument(std::string owner, Symbols::FunctionSymbol* method) :
    m_owner(owner),
    m_method(method),
    m_is_type_init(method == nullptr) {}

std::string MethodCallInstructionArgument::owner() const {
    return m_owner;
}

std::string MethodCallInstructionArgument::method_id() const {
    return m_is_type_init ? "<init>" : m_method->name();
}

std::string MethodCallInstructionArgument::parameter_type_ids() const {
    if (m_is_type_init || m_method->params().empty())
        return "()";
    std::string params = "(";
    for (auto param : m_method->params()) {
        params += param->type()->jasm_str();
    }
    params += ")";
    return params;
}

std::string MethodCallInstructionArgument::return_type_id() const {
    if (m_is_type_init) return Symbols::TypeSymbol::Void.jasm_str();
    return m_method->type()->jasm_str();
}

InstructionArgumentKind MethodCallInstructionArgument::kind() const {
    return InstructionArgumentKind::Method_Call;
}

std::string MethodCallInstructionArgument::str() const {
    std::string name = method_id();
    std::string params = parameter_type_ids();
    std::string type = return_type_id();
    return "Method " + owner() + " " + name + " " + params + type;
}

}
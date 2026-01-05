#include "instruction.h"
#include "instruction_arguments.h"
#include "jvm_opcode_info.h"
#include "jvm_opcodes.h"

namespace JVMProcessor {

Instruction::Instruction(JVMOpCode op_code, InstructionArgument *argument)
    : m_op_code(op_code), m_argument(argument), m_has_label(false) {}

Instruction::Instruction(JVMOpCode op_code)
    : m_op_code(op_code), m_argument(new EmptyInstructionArgument()),
      m_has_label(false) {}

JVMOpCode Instruction::op_code() const {
  return m_has_label ? JVMOpCode::nop : m_op_code;
}

InstructionArgument *Instruction::argument() const { return m_argument; }

std::string Instruction::op_code_str() const {
  return OpCodeInfo::op_code_name(m_op_code);
}

std::string Instruction::str() const {
  std::string indent = "";
  std::string op = op_code_str();
  if (m_has_label) {
    std::string pref = m_opt_label_tag + ":";
    // just gonna brute force it, we will probably
    // never have 99+ labels in one method.
    std::size_t size = pref.size();
    if (size == 3)
      indent = "   ";
    else if (size == 4)
      indent = "  ";
    else if (size == 5)
      indent = " ";

    op = pref + indent + op;
  } else {
    op = "      " + op;
  }
  if (m_argument->kind() == InstructionArgumentKind::Empty)
    return op;
  return op + " " + m_argument->str();
}

bool Instruction::has_label() const { return m_has_label; }

void Instruction::set_label(std::string new_label) {
  m_opt_label_tag = new_label;
  m_has_label = true;
}

} // namespace JVMProcessor
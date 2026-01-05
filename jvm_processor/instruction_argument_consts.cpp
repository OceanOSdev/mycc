#include "instruction_arguments.h"

namespace JVMProcessor {

ITupleConstInstructionArgument::ITupleConstInstructionArgument(int val_one,
                                                               int val_two)
    : m_ival_one(val_one), m_ival_two(val_two) {}

InstructionArgumentKind ITupleConstInstructionArgument::kind() const {
  return InstructionArgumentKind::Integer_Tuple;
}

int ITupleConstInstructionArgument::first_value() const { return m_ival_one; }

int ITupleConstInstructionArgument::second_value() const { return m_ival_two; }

std::string ITupleConstInstructionArgument::str() const {
  return std::to_string(m_ival_one) + " " + std::to_string(m_ival_two);
}

// ================
// Char Const
// ================

CConstInstructionArgument::CConstInstructionArgument(char val) : m_cval(val) {}

char CConstInstructionArgument::value() const { return m_cval; }

InstructionArgumentKind CConstInstructionArgument::kind() const {
  return InstructionArgumentKind::Char_Constant;
}

std::string CConstInstructionArgument::str() const {
  int ival = (int)m_cval; // ldc is weird about chars
  return std::to_string(ival);
}

// ================
// Int Const
// ================

IConstInstructionArgument::IConstInstructionArgument(int val) : m_ival(val) {}

int IConstInstructionArgument::value() const { return m_ival; }

InstructionArgumentKind IConstInstructionArgument::kind() const {
  return InstructionArgumentKind::Integer_Constant;
}

std::string IConstInstructionArgument::str() const {
  return std::to_string(m_ival);
}

// ================
// Float Const
// ================

FConstInstructionArgument::FConstInstructionArgument(float val) : m_fval(val) {}

float FConstInstructionArgument::value() const { return m_fval; }

InstructionArgumentKind FConstInstructionArgument::kind() const {
  return InstructionArgumentKind::Float_Constant;
}

std::string FConstInstructionArgument::str() const {
  std::string ret = std::to_string(m_fval);
  return m_fval > 0 ? "+" + ret : ret;
}

// ================
// String Const
// ================

SConstInstructionArgument::SConstInstructionArgument(std::string val)
    : m_sval(val) {}

std::string SConstInstructionArgument::value() const { return m_sval; }

InstructionArgumentKind SConstInstructionArgument::kind() const {
  return InstructionArgumentKind::String_Constant;
}

std::string SConstInstructionArgument::str() const {
  return "'" + m_sval + "'";
}

} // namespace JVMProcessor
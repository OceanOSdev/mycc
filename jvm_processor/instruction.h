#pragma once
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <string>

namespace JVMProcessor {

enum class JVMOpCode;
class InstructionArgument;

class Instruction {
private:
  JVMOpCode m_op_code;
  InstructionArgument *m_argument;
  std::string m_opt_label_tag;
  bool m_has_label;

public:
  Instruction(std::string label_tag);
  Instruction(JVMOpCode op_code, InstructionArgument *argument);
  Instruction(JVMOpCode op_code);

  /**
   * @brief The op code belonging to this instruction.
   */
  JVMOpCode op_code() const;

  /**
   * @brief The argument taken by the op code in
   * this instruction.
   */
  InstructionArgument *argument() const;

  /**
   * @brief The op code's mnemonic.
   */
  std::string op_code_str() const;

  /**
   * @brief This instruction's string representation.
   *
   * @return A java assembly formatted string representation of this
   * instruction.
   */
  std::string str() const;

  /**
   * @brief Whether or not this instruction is prefixed with a label.
   */
  bool has_label() const;

  /**
   * @brief Prefix this instruction with a label.
   *
   * @param new_label The new label to prefix the instruction with.
   */
  void set_label(std::string new_label);
};

} // namespace JVMProcessor

#endif
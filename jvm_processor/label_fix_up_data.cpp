#include "label_fix_up_data.h"
#include "../binding/bound_label_statement_node.h"

namespace JVMProcessor {

LabelFixUpData::LabelFixUpData(int instr_idx, Binding::BoundLabel *label)
    : m_instruction_index(instr_idx), m_label(label) {}

int LabelFixUpData::instruction_index() const { return m_instruction_index; }

Binding::BoundLabel *LabelFixUpData::label() const { return m_label; }

} // namespace JVMProcessor
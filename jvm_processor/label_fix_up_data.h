#pragma once
#ifndef LABEL_FIX_UP_DATA_H
#define LABEL_FIX_UP_DATA_H

namespace Binding {
    class BoundLabel;
}

namespace JVMProcessor {

class LabelFixUpData {
private:
    int m_instruction_index;
    Binding::BoundLabel* m_label;
public:
    LabelFixUpData(int instr_idx, Binding::BoundLabel* label);

    int instruction_index() const;

    Binding::BoundLabel* label() const;
};

}

#endif
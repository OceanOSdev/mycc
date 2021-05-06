#pragma once
#ifndef BOUND_LABEL_STATEMENT_NODE_H
#define BOUND_LABEL_STATEMENT_NODE_H

#include "bound_statement_node.h"
#include <string>

namespace JVMProcessor {
    class JAsmBuilder;
}

namespace Binding {

class BoundLabel {
private:
    std::string m_name;
    friend class JVMProcessor::JAsmBuilder;
public:
    BoundLabel(std::string name);

    std::string name() const;
};

class BoundLabelStatementNode : public BoundStatementNode {
private:
    BoundLabel* m_label;
public:
    BoundLabelStatementNode(BoundLabel* label);

    BoundLabel* label() const;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;

};

}


#endif
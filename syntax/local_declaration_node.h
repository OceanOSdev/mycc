#pragma once
#ifndef SYNTAX_LOCAL_DECLARATION_NODE_H
#define SYNTAX_LOCAL_DECLARATION_NODE_H

#include "statement_node.h"

namespace Syntax {

class LocalDeclarationNode : public StatementNode {
public:
    LocalDeclarationNode() {}
};

}

#endif
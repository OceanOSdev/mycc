#pragma once
#ifndef QSEM_H
#define QSEM_H

#include "syntax/syntax_node.h"
#include <string>

namespace QuickSemanticAnalyzer {
    void analyze(Syntax::SyntaxNode* n);
}

#endif
#pragma once
#ifndef QSEM_H
#define QSEM_H

namespace Syntax {class SyntaxNode;}
#include <string>

namespace QuickSemanticAnalyzer {
    void analyze(Syntax::SyntaxNode* n);
}

#endif
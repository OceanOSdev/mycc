#pragma once
#ifndef SYNTAX_TREE_PRINTER_H
#define SYNTAX_TREE_PRINTER_H

#include "syntax/syntax_node.h"
#include <string>

namespace SyntaxTreePrinter {
    void print_nodes(Syntax::SyntaxNode* n);
}

#endif
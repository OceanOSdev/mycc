#pragma once
#ifndef SYNTAX_NODE_H
#define SYNTAX_NODE_H

namespace Syntax {
class SyntaxToken;

class SyntaxNode {

public:
    SyntaxNode() {}
    virtual ~SyntaxNode() = default;
};

}

#endif
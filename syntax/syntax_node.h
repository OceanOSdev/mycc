#pragma once
#ifndef SYNTAX_NODE_H
#define SYNTAX_NODE_H

namespace Syntax {
class SyntaxToken;
enum SyntaxKind: int;

class SyntaxNode {

public:
    SyntaxNode() {}
    virtual ~SyntaxNode() = default;

    /*
     * What kind of syntax node this is.
     */
    virtual SyntaxKind kind() const = 0;
};

}

#endif
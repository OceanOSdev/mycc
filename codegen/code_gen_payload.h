#pragma once
#ifndef CODE_GEN_PAYLOAD_H
#define CODE_GEN_PAYLOAD_H

#include <string>
#include <vector>

namespace Binding {
    class BoundGlobalDeclarationNode;
}

namespace CodeGen {

class CodeGenPayload {
private:
    std::string m_filename;
    std::vector<Binding::BoundGlobalDeclarationNode*> m_lowered_global_declarations;
    CodeGenPayload(std::string filename, std::vector<Binding::BoundGlobalDeclarationNode*> bound_declarations);
public:
    /**
     * Creates a payload object to send to the bytecode emitter. 
     * Runs the tree rewriter on the BoundGlobalDeclarationNodes.
     * 
     * Assumes filename ends in '.c'
     * 
     * @param filename The name of the source code file.
     * @param bound_declarations The list of bound declaration nodes from the binder.
     */
    static CodeGenPayload* create_payload(std::string filename, std::vector<Binding::BoundGlobalDeclarationNode*> bound_declarations);

    /**
     * @brief The output filename.
     */
    std::string output_filename() const;

    /**
     * @brief A list of the rewritten bound nodes.
     */
    std::vector<Binding::BoundGlobalDeclarationNode*> lowered_global_declarations() const;

};

}

#endif
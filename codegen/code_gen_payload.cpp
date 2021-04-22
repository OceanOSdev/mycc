#include "code_gen_payload.h"
#include "../binding/tree_rewriter.h"

namespace CodeGen {

CodeGenPayload::CodeGenPayload(std::string filename, std::vector<Binding::BoundGlobalDeclarationNode*> bound_declarations) :
    m_filename(filename), m_lowered_global_declarations(bound_declarations) {}


/**
 * Creates a payload object to send to the bytecode emitter. 
 * Runs the tree rewriter on the BoundGlobalDeclarationNodes.
 * 
 * Assumes filename ends in '.c'
 * 
 * @param filename The name of the source code file.
 * @param bound_declarations The list of bound declaration nodes from the binder.
 */
CodeGenPayload* CodeGenPayload::create_payload(std::string filename, std::vector<Binding::BoundGlobalDeclarationNode*> bound_declarations) {
    std::string output_filename = filename;
    output_filename.replace(output_filename.end()-1, output_filename.end(), 1, 'j');
    std::vector<Binding::BoundGlobalDeclarationNode*> lowered_program = Binding::TreeRewriter::rewrite(bound_declarations);

    return new CodeGenPayload(output_filename, lowered_program);
}

/**
 * @brief The output filename.
 */
std::string CodeGenPayload::output_filename() const {
    return m_filename;
}

/**
 * @brief A list of the rewritten bound nodes.
 */
std::vector<Binding::BoundGlobalDeclarationNode*> CodeGenPayload::lowered_global_declarations() const {
    return m_lowered_global_declarations;
}


}
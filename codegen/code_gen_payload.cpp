#include "code_gen_payload.h"
#include "../binding/tree_rewriter.h"

namespace CodeGen {

CodeGenPayload::CodeGenPayload(std::string in_filename, std::string out_filename, std::vector<Binding::BoundGlobalDeclarationNode*> bound_declarations) :
    m_in_filename(in_filename),
    m_out_filename(out_filename), 
    m_lowered_global_declarations(bound_declarations) {}


CodeGenPayload* CodeGenPayload::create_payload(std::string filename, std::vector<Binding::BoundGlobalDeclarationNode*> bound_declarations) {
    auto filename_base = filename.substr(0, filename.find("."));
    return create_payload(filename, filename_base + ".j", bound_declarations);
}

CodeGenPayload* CodeGenPayload::create_payload(std::string in_filename, std::string out_filename, std::vector<Binding::BoundGlobalDeclarationNode*> bound_declarations) {
    std::vector<Binding::BoundGlobalDeclarationNode*> lowered_program = Binding::TreeRewriter::rewrite(bound_declarations);
    return new CodeGenPayload(in_filename, out_filename, lowered_program);
}


std::string CodeGenPayload::output_filename() const {
    return m_out_filename;
}

std::string CodeGenPayload::filename_base() const {
    ///TODO: maybe make this its own instance field.
    std::size_t slash_idx = m_in_filename.find_last_of('/');
    std::size_t start_idx = slash_idx == std::string::npos ? 0 : slash_idx + 1;
    std::string no_slash_file_name = m_in_filename.substr(start_idx);
    return no_slash_file_name.substr(0, no_slash_file_name.find("."));
}

std::vector<Binding::BoundGlobalDeclarationNode*> CodeGenPayload::lowered_global_declarations() const {
    return m_lowered_global_declarations;
}


}
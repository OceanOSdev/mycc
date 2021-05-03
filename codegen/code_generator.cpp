#include "code_generator.h"
#include "code_gen_payload.h"

namespace CodeGen {

CodeGenerator::CodeGenerator(CodeGenPayload* payload, bool debug) :
    m_payload(payload), m_debug_mode(debug) {

    if (debug)
        m_outstream = &std::cout; // override any output option in the payload
    else
        m_outstream = new std::ofstream(payload->output_filename());
}

bool CodeGenerator::emit() {
    return false;
}

bool CodeGenerator::is_debug() const {
    return m_debug_mode;
}


}
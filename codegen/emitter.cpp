#include "emitter.h"
#include "code_gen_payload.h"

namespace CodeGen {

Emitter::Emitter(CodeGenPayload* payload, bool debug) :
    m_payload(payload), m_debug_mode(debug) {

    if (debug)
        m_outstream = &std::cout; // override any output option in the payload
    else
        m_outstream = new std::ofstream(payload->output_filename());
}

bool Emitter::emit() {
    return false;
}

bool Emitter::is_debug() const {
    return m_debug_mode;
}


}
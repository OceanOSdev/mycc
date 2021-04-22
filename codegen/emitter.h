#pragma once
#ifndef EMITTER_H
#define EMITTER_H

#include <fstream>
#include <ostream>
#include <iostream>

namespace CodeGen {
class CodeGenPayload;
class Emitter {
private:
    std::ostream* m_outstream;
    CodeGenPayload* m_payload;
    bool m_debug_mode;
public:
    Emitter(CodeGenPayload* payload, bool debug);

    bool emit();

    bool is_debug() const;
};

}

#endif
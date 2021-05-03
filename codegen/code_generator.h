#pragma once
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <fstream>
#include <ostream>
#include <iostream>

namespace CodeGen {
class CodeGenPayload;
class CodeGenerator {
private:
    std::ostream* m_outstream;
    CodeGenPayload* m_payload;
    bool m_debug_mode;

    bool emit_global_variable_declarations();
    bool emit_global_struct_declarations();
    bool emit_local_struct_declarations();
public:
    CodeGenerator(CodeGenPayload* payload, bool debug);

    bool emit();

    bool is_debug() const;
};

}

#endif
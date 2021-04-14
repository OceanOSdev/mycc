#pragma once
#ifndef BOUND_PROGRAM_H
#define BOUND_PROGRAM_H

#include <unordered_map>
#include <vector>
#include <string>

namespace Symbols {
    class FunctionSymbol;
}

namespace Binding {

class BoundBlockStatement;

class BoundProgram {
private:
    std::vector<std::string> m_diagnostics;
    Symbols::FunctionSymbol* m_main_function;
    std::unordered_map<Symbols::FunctionSymbol*, BoundBlockStatement*> m_functions;


};

}

#endif
#pragma once
#ifndef BOUND_PROGRAM_H
#define BOUND_PROGRAM_H

#include <unordered_map>
#include <vector>
#include <string>

namespace Binding {
class BoundGlobalDeclarationNode;

class BoundProgram {
private:
    std::vector<std::string> m_diagnostics;
    std::vector<BoundGlobalDeclarationNode*> m_global_statements;
    
    
};

}

#endif
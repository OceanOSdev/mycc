#pragma once
#ifndef STRUCT_SYMBOL_H
#define STRUCT_SYMBOL_H

#include <vector>
#include <string>

namespace Symbols {
class VariableSymbol;

class StructSymbol {
private:
    std::string m_name;
    std::vector<VariableSymbol*> m_members;
public:
    StructSymbol(std::string name, std::vector<VariableSymbol*> members);
    ~StructSymbol();

    /*
     * Name identifier of this struct type.
     */
    std::string name() const;

    /*
     * List of the members of this struct type.
     */
    std::vector<VariableSymbol*> members() const;
};

}

#endif
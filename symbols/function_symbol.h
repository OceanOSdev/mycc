#pragma once
#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H

#include <vector>
#include <string>

namespace Symbols {
class TypeSymbol;
class ParameterSymbol;

class FunctionSymbol {
private:
    std::string m_name;
    TypeSymbol* m_type;
    std::vector<ParameterSymbol*> m_params;
public:
    FunctionSymbol(std::string name, TypeSymbol* type, std::vector<ParameterSymbol*> params);
    ~FunctionSymbol();

    /*
     * The name identifier for this function.
     */
    std::string name() const;

    /*
     * The type of value that this function returns.
     */
    TypeSymbol* type() const;

    /*
     * The list of paramters that this function takes in.
     */
    std::vector<ParameterSymbol*> params() const;
};

}

#endif
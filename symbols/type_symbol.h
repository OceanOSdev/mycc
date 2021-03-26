#ifndef TYPE_SYMBOL_H
#define TYPE_SYMBOL_H

#include <string>

namespace Symbols {

class TypeSymbol {
private:
    std::string m_identifier;
    bool m_is_struct;
    bool m_is_const;
public:
    TypeSymbol(std::string id, bool is_struct = false, bool is_const = false) :
        m_identifier(id), m_is_struct(is_struct), m_is_const(is_const) {}

    ~TypeSymbol() {}

    /*
     * The name of the type.
     */
    std::string name() const { return m_identifier; }

    /*
     * Whether or not the type is a struct.
     */
    bool is_struct() const { return m_is_struct; }

    /*
     * Whether or not the type is read only.
     */
    bool is_const() const { return m_is_const; }

};

}

#endif
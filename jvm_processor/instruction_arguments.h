#pragma once
#ifndef INSTRUCTION_ARGUMENTS_H
#define INSTRUCTION_ARGUMENTS_H

#include <cstdint>
#include <vector>
#include <string>

namespace Symbols {
    class FunctionSymbol;
    class TypeSymbol;
    class StructSymbol;
    class VariableSymbol;
}

namespace Binding {
    class BoundLabel;
}

namespace JVMProcessor {

enum InstructionArgumentKind : uint16_t {
    Empty = 0b0,
    Label = 0b1,
    Integer_Tuple = 0b10,
    Char_Constant = 0b100,
    Integer_Constant = 0b1000,
    Float_Constant = 0b10000,
    String_Constant = 0b100000,
    Type_Identifier = 0b1000000,
    Field_Accessor = 0b10000000,
    Method_Call = 0b100000000,

    //Helper vals
    Any_Constant = Char_Constant | Integer_Constant | Float_Constant | String_Constant
};

class InstructionArgument {
public:
    /**
     * @brief What kind of instruction argument this is.
     */
    virtual InstructionArgumentKind kind() const = 0;

    /**
     * @brief String representation of this instruction argument.
     * 
     * Formatted as java assembly code.
     */
    virtual std::string str() const = 0;
};

/**
 * @brief A class that represents an empty argument,
 * used in cases where an instruction's op code takes no
 * arguments (e.g. pop, iadd). 
 * 
 * @note: since args are actually vectors now, maybe this is
 *      no longer necessary.
 */
class EmptyInstructionArgument : public InstructionArgument {
public:
    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents a label-valued argument to an 
 * instruction's op code. 
 */
class LabelInstructionArgument : public InstructionArgument {
private:
    Binding::BoundLabel* m_label;
public:
    LabelInstructionArgument(Binding::BoundLabel* label);

    /**
     * @brief The label name.
     */
    std::string label() const;

    Binding::BoundLabel* bound_label() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents a tuple int-valued argument to an
 * instruction's op code. 
 * 
 * Exists because of op codes like iinc.
 */
class ITupleConstInstructionArgument : public InstructionArgument {
private:
    int m_ival_one;
    int m_ival_two;
public:
    ITupleConstInstructionArgument(int val_one, int val_two);

    /**
     * @brief The first integer constant value. 
     */
    int first_value() const;
    
    /**
     * @brief The second integer constant value. 
     */
    int second_value() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents an char-valued argument to an
 * instruction's op code. 
 */
class CConstInstructionArgument : public InstructionArgument {
private:
    char m_cval;
public:
    CConstInstructionArgument(char val);

    /**
     * @brief The character constant value. 
     */
    char value() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents an int-valued argument to an
 * instruction's op code. 
 */
class IConstInstructionArgument : public InstructionArgument {
private:
    int m_ival;
public:
    IConstInstructionArgument(int val);

    /**
     * @brief The integer constant value. 
     */
    int value() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents a float-valued argument to an
 * instruction's op code. 
 */
class FConstInstructionArgument : public InstructionArgument {
private:
    float m_fval;
public:
    FConstInstructionArgument(float val);

    /**
     * @brief The floating point constant value. 
     */
    float value() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents a string-valued argument to an
 * instruction's op code. 
 */
class SConstInstructionArgument : public InstructionArgument {
private:
    std::string m_sval;
public:
    SConstInstructionArgument(std::string val);

    /**
     * @brief The string constant value. 
     */
    std::string value() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents a reference-type-valued argument
 * to an instruction's op code.
 * 
 * Only use case I can think of at the moment is for the
 * 'new' op code.
 */
class TypeIdInstructionArgument : public InstructionArgument {
private:
    const Symbols::TypeSymbol* m_type;
public:
    TypeIdInstructionArgument(const Symbols::TypeSymbol* type);

    /**
     * @brief The type name, as represented in java assembly.
     */
    std::string type_id() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents arguments passed into the 
 * 'getfield' instruction. 
 */
class FieldAccessorInstructionArgument : public InstructionArgument {
private:
    Symbols::StructSymbol* m_struct;
    Symbols::VariableSymbol* m_member;
public:
    FieldAccessorInstructionArgument(Symbols::StructSymbol* _struct, Symbols::VariableSymbol* member);
    /**
     * @brief Field accessor instruction argument in the event that we're actually just accessing
     * a global variable in the original source code. 
     * 
     * @param filename_base The base filename (no extensions) of the global variable container.
     * @param member The variable symbol of the global variable.
     */
    FieldAccessorInstructionArgument(std::string filename_base, Symbols::VariableSymbol* member);

    /**
     * @brief The encapsulating struct name.
     */
    std::string encapsulating_type_id() const;

    /**
     * @brief The name of the struct member.
     */
    std::string member_id() const;

    /**
     * @brief A string representation of the struct member's type,
     * as represented in java assembly code.
     */
    std::string member_type_id() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

/**
 * @brief Represents arguments passed into the 
 * 'invoke' family of op codes. 
 */
class MethodCallInstructionArgument : public InstructionArgument {
private:
    std::string m_owner;
    Symbols::FunctionSymbol* m_method;
    bool m_is_type_init;
public:
    MethodCallInstructionArgument(std::string owner, Symbols::FunctionSymbol* method);

    /**
     * @brief The name of the class that owns this method.
     * 
     * In most cases will just be the input file name, but
     * in the cases of <init> or the libc methods, will be different.
     */
    std::string owner() const;

    /**
     * @brief The name of the method.
     */
    std::string method_id() const;

    /**
     * @brief A string representation of the parameter types,
     * as represented in java assembly code.
     */
    std::string parameter_type_ids() const;

    /**
     * @brief A string representation of the return type,
     * as represented in java assembly code.
     */
    std::string return_type_id() const;

    InstructionArgumentKind kind() const override;

    std::string str() const override;
};

}

#endif
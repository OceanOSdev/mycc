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

namespace JVMProcessor {

enum InstructionArgumentKind : uint8_t {
    Empty,
    Label,
    Integer_Constant,
    Float_Constant,
    Type_Identifier,
    Field_Accessor,
    Method_Call
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
    std::string m_label;
public:
    LabelInstructionArgument(std::string label);

    /**
     * @brief The label name.
     */
    std::string label() const;

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
#pragma once
#ifndef TYPE_SYMBOL_H
#define TYPE_SYMBOL_H

#include "symbol.h"
#include <compare>
#include <string>

namespace Symbols {

using TypeAttribute = struct TypeAttribute;
struct TypeAttribute {
  bool is_struct = false;
  bool is_integer_type = false;
  bool is_numeric_type = false;
  bool is_array = false;
  bool is_const = false;
  TypeAttribute(bool is_strct = false, bool is_int = false, bool is_num = false,
                bool is_arr = false, bool is_c = false);
  std::partial_ordering operator<=>(const TypeAttribute &other) const;
};

class TypeSymbol : public Symbol {
private:
  TypeAttribute m_attributes;

public:
  TypeSymbol(std::string id, TypeAttribute attributes);

  ~TypeSymbol();

  /*
   * Attributes related to this type.
   */
  TypeAttribute attributes() const;

  /*
   * The type of symbol this is.
   */
  SymbolKind kind() const override;

  /*
   * Returns a copy of the current type symbol
   * but with the array attribute true.
   */
  const TypeSymbol *as_array_type() const;

  /*
   * Returns a copy of the current type symbol
   * but with the array attribute false.
   *
   * Used mostly just for grabbing the type of an array
   * item in a bound index expression
   */
  const TypeSymbol *as_array_element_type() const;

  /*
   * Returns a copy of the current type symbol
   * but with the const attribute true.
   */
  const TypeSymbol *as_const_type() const;

  /*
   * Returns a copy of the current type symbol
   * but with the const attribute false.
   */
  const TypeSymbol *as_mutable_type() const;

  std::partial_ordering operator<=>(const TypeSymbol &other) const;

  /*
   * String representing type including attributes.
   */
  std::string str() const;

  /**
   * @brief String representation of type in java assembly format.
   *
   * @param is_in_method_sig Whether to format as param/ret type
   *      (i.e. prepend 'L' and append ';' to object types).
   */
  std::string jasm_str(bool is_in_method_sig = true) const;

  /*
   * Grabs an instance of TypeSymbol if it matches one of the
   * primitive types:
   *
   * - Char
   *
   * - Int
   *
   * - Float
   *
   * Returns Error otherwise.
   */
  static const TypeSymbol *try_lookup_primitive(std::string ident);
  static const TypeSymbol *try_lookup_primitive_function(std::string ident);
  static const TypeSymbol Void;
  static const TypeSymbol Char;
  static const TypeSymbol Int;
  static const TypeSymbol Float;
  static const TypeSymbol String;
  static const TypeSymbol Error;
  static const TypeSymbol *get_wider_type(const TypeSymbol *lhs,
                                          const TypeSymbol *rhs);

  /*
   * Determines whether the lhs type is the same as the rhs type.
   *
   * OR
   *
   * whether the lhs type can be implicitly widened to the rhs type.
   */
  static bool are_types_equivalent(const TypeSymbol *lhs,
                                   const TypeSymbol *rhs);
  /*
   * Determines whether the lhs type is the same as the rhs type.
   */
  static bool are_types_equal(const TypeSymbol *lhs, const TypeSymbol *rhs);
  /*
   * Whether or not we will need to explicitly cast from type "from" to type
   * "to" when we generate bytecode.
   *
   * Should be used while rewriting the bound tree to deal with cases where an
   * integer type needs to be widened to a non-integer numeric type.
   */
  static bool requires_bytecode_cast(const TypeSymbol *from,
                                     const TypeSymbol *to);
  static bool is_error_type(const TypeSymbol *type);
  static bool is_error_or_incomplete_type(const TypeSymbol *type);
  /*
   * Whether the given type can be used as a conditional type in
   * an if statement, for loop, while loop, do while, or ternary
   * expression.
   */
  static bool is_conditional_type(const TypeSymbol *type);
  static bool can_be_explicitly_casted(const TypeSymbol *from,
                                       const TypeSymbol *to);

  /*
   * Whether or not during bytecode emitting if using 'type' requires the
   * 'a' prefix.
   *
   * Note: The 'Void' and 'Error' types are considered "reference types" in
   * this scenario. The decision was completely arbitrary.
   */
  static bool is_bytecode_reference_type(const TypeSymbol *type);
};

} // namespace Symbols

#endif
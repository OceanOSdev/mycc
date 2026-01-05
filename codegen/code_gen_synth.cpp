#include "code_gen_payload.h"
#include "code_generator.h"
#include "method_reference.h"

#include "../jvm_processor/finalized_body.h"
#include "../jvm_processor/j_asm_builder.h"

#include "../symbols/function_symbol.h"
#include "../symbols/symbol_factory.h"
#include "../symbols/type_symbol.h"

#include "../binding/bound_assignment_expression_node.h"
#include "../binding/bound_expression_statement_node.h"
#include "../binding/bound_node_factory.h"
#include "../binding/bound_variable_group_declaration_node.h"

namespace CodeGen {

void CodeGenerator::synthesize_init_method() {
  // since this will always be the same for the main file,
  // we just directly emit it to the compiled method bodies
  // map (I know this feels kind of hacky, and it definitely is,
  // but for now it should suffice at least while I work on the
  // rest of the emitter. [also I don't know why I keep switching
  // saying "we" and saying "I"...])

  std::vector<std::string> init_body{
      "aload_0", "invokespecial Method java/lang/Object <init> ()V", "return"};
  int local_count = 1;
  int stack_size = 1;
  auto finalized_body =
      new JVMProcessor::FinalizedBody(init_body, local_count, stack_size);
  auto init_method = look_up_method("<init>");
  m_compiled_method_bodies[init_method->method()] = finalized_body;
  m_methods_to_emit.push_back(init_method);
}

void CodeGenerator::synthesize_clinit_method() {
  std::vector<Binding::BoundStatementNode *> statements;
  for (auto field : m_fields_needing_clinit) {
    auto expression = Binding::Factory::assignment(field->variable_symbol(),
                                                   field->initializer());
    statements.push_back(Binding::Factory::statement(expression));
  }

  auto clinit_method_old = look_up_method("<clinit>");
  auto clinit_method_new =
      clinit_method_old->define_method(Binding::Factory::block(statements));
  update_method_in_method_map(clinit_method_new);
  m_methods_to_emit.push_back(clinit_method_new);
}

std::vector<std::string> gen_part_four_body(std::string main_class) {
  return {
      "invokestatic Method " +
          JVMProcessor::JAsmBuilder::str_as_emittable_identifier(main_class) +
          " main ()I",
      "istore_1",
      "getstatic Field java/lang/System out Ljava/io/PrintStream;",
      "ldc 'Return code: '",
      "invokevirtual Method java/io/PrintStream print (Ljava/lang/String;)V",
      "getstatic Field java/lang/System out Ljava/io/PrintStream;",
      "iload_1",
      "invokevirtual Method java/io/PrintStream println (I)V",
      "return"};
}

std::vector<std::string> gen_part_five_body(std::string main_class) {
  return {
      "invokestatic Method " +
          JVMProcessor::JAsmBuilder::str_as_emittable_identifier(main_class) +
          " main ()I",
      "pop", "return"};
}

void CodeGenerator::synthesize_j_main_method() {
  namespace s_factory = Symbols::Factory;
  int local_count = 1;
  int stack_size = 1;
  auto owner = m_payload->filename_base();
  auto body = gen_part_five_body(owner);
  if (m_emit_part_four_info) {
    local_count++;
    stack_size++;
    body = gen_part_four_body(owner);
  }
  auto finalized_body =
      new JVMProcessor::FinalizedBody(body, local_count, stack_size);

  // Really scummy way to do this, but just recreate the symbol to search
  auto j_str_type = s_factory::type("java/lang/String");
  auto j_main_symbol = s_factory::function("main", s_factory::void_type(),
                                           {j_str_type->as_array_type()});
  auto main_method = look_up_method(j_main_symbol);
  m_compiled_method_bodies[main_method->method()] = finalized_body;
  m_methods_to_emit.push_back(main_method);
}

} // namespace CodeGen
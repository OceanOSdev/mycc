#include "code_generator.h"
#include "code_gen_payload.h"
#include "method_reference.h"

#include "../jvm_processor/finalized_body.h"
#include "../jvm_processor/instruction.h"
#include "../jvm_processor/j_asm_builder.h"

#include "../binding/bound_block_statement_node.h"
#include "../binding/bound_function_definition_node.h"
#include "../binding/bound_global_declaration_node.h"
#include "../binding/bound_global_statement_node.h"
#include "../binding/bound_statement_node.h"
#include "../binding/bound_struct_declaration_node.h"
#include "../binding/bound_variable_group_declaration_node.h"

#include "../symbols/function_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"

#include "../symbols/symbol_factory.h"

namespace CodeGen {

CodeGenerator::CodeGenerator(CodeGenPayload *payload, bool debug,
                             bool emit_part_four_info)
    : m_payload(payload), m_debug_mode(debug),
      m_emit_part_four_info(emit_part_four_info) {

  if (debug)
    m_outstream = &std::cout; // override any output option in the payload
  else
    m_outstream = new std::ofstream(payload->output_filename());
}

bool CodeGenerator::emit() {
  const std::string indent = "   ";
  std::string code_indent = indent + indent;
  bool first = true;
  init();
  emit_required_methods();
  using jvm_emit = JVMProcessor::JAsmBuilder;
  for (auto method : m_methods_to_emit) {
    auto symbol = method->method();
    if (!m_compiled_method_bodies.contains(symbol))
      continue; // hasnt been compiled yet, only here for debug.
    auto finalized = m_compiled_method_bodies[symbol];
    auto method_ref = look_up_method(
        symbol); // this is on average an O(n) call, for large files this can
                 // get problematic, should probably refactor.
    bool is_static = method_ref->is_static();

    if (!first) {
      std::cout << '\n';
    } else {
      first = !first;
    }

    (*m_outstream) << jvm_emit::emit_method_definition_header(symbol, is_static)
                   << std::endl;
    (*m_outstream) << indent
                   << jvm_emit::emit_method_code_directive(
                          finalized->max_stack_size(), finalized->local_count())
                   << std::endl;

    if (!finalized->is_hard_coded()) {
      for (auto instruction : finalized->body()) {
        if (instruction->has_label())
          (*m_outstream) << '\n';
        (*m_outstream) << instruction->str() << '\n';
      }
    } else {
      for (auto instruction : finalized->hard_coded_body()) {
        (*m_outstream) << indent << indent << instruction << '\n';
      }
    }

    (*m_outstream) << std::endl;
    (*m_outstream) << indent
                   << jvm_emit::emit_directive_end(
                          JVMProcessor::BlockDirectiveType::CodeDirective)
                   << std::endl;
    (*m_outstream) << jvm_emit::emit_directive_end(
                          JVMProcessor::BlockDirectiveType::MethodDirective)
                   << std::endl;
  }

  (*m_outstream) << jvm_emit::emit_directive_end(
                        JVMProcessor::BlockDirectiveType::ClassDirective)
                 << std::endl;
  return true;
}

void CodeGenerator::init() {
  resolve_methods();
  emit_basic_assembly_metadata(m_payload->filename_base());
  synthesize_init_method();
  for (auto decl : m_payload->lowered_global_declarations()) {
    auto kind = decl->kind();
    if (kind == Binding::BoundNodeKind::GlobalStatement) {
      auto gdec = dynamic_cast<Binding::BoundGlobalStatementNode *>(decl);
      if (gdec->statement()->kind() ==
          Binding::BoundNodeKind::VariableGroupDeclaration) {
        auto group_decl =
            dynamic_cast<Binding::BoundVariableGroupDeclarationNode *>(
                gdec->statement());
        for (auto var_decl : group_decl->variable_declarations())
          emit_field(var_decl, true);
      } else {
        auto struct_decl = dynamic_cast<Binding::BoundStructDeclarationNode *>(
            gdec->statement());
        emit_struct_declaration(struct_decl);
      }
    } else {
      auto bfdecl = dynamic_cast<Binding::BoundFunctionDefinitionNode *>(decl);
      auto method = look_up_method(bfdecl->function_symbol());
      m_methods_to_emit.push_back(method);
    }
  }
  synthesize_j_main_method();
  if (!m_fields_needing_clinit.empty())
    synthesize_clinit_method();
}

void CodeGenerator::emit_required_methods() {
  for (auto method : m_methods_to_emit) {
    if (method->is_defined()) {
      emit_method(method);
    }
  }
}

bool CodeGenerator::is_debug() const { return m_debug_mode; }

void CodeGenerator::emit_struct_declaration(
    __attribute__((unused)) Binding::BoundStructDeclarationNode *struct_decl) {}

void CodeGenerator::emit_method(MethodReference *method) {
  m_builder = new JVMProcessor::JAsmBuilder();
  bool implicit_return = Symbols::TypeSymbol::are_types_equal(
      method->method()->type(), Symbols::Factory::void_type());
  for (auto param : method->method()->params()) {
    // declare the parameters as the first locals.
    declare_local(Symbols::VariableSymbol::from_parameter_symbol(param));
  }
  emit_statement(method->body());
  if (implicit_return)
    m_builder->emit_return_op_code(); // add implicit return to void methods.

  auto finalized = m_builder->finalize();
  m_compiled_method_bodies[method->method()] = finalized;
}

void CodeGenerator::emit_field(Binding::BoundVariableDeclarationNode *field,
                               bool is_static) {
  bool needs_clinit;
  std::string output = JVMProcessor::JAsmBuilder::emit_field_directive(
      field, is_static, needs_clinit);
  if (needs_clinit)
    m_fields_needing_clinit.push_back(field);

  (*m_outstream) << output << std::endl;
}

void CodeGenerator::emit_basic_assembly_metadata(std::string class_name) {
  (*m_outstream) << JVMProcessor::JAsmBuilder::emit_class_directive(class_name)
                 << std::endl;
  (*m_outstream) << JVMProcessor::JAsmBuilder::emit_super_directive()
                 << std::endl;
}

void CodeGenerator::resolve_methods() {
  namespace s_factory = Symbols::Factory;
  // create function symbols for libc functions
  auto putchar_symbol = s_factory::function("putchar", s_factory::int_type(),
                                            {s_factory::int_type()});
  auto getchar_symbol =
      s_factory::simple_function("getchar", s_factory::int_type());

  /// TODO: resolve such methods for structs.
  auto _init_method_reference = MethodReference::resolve_method(
      s_factory::init_function(), m_payload->filename_base(), false);
  auto _clinit_method_reference = MethodReference::resolve_method(
      s_factory::clinit_function(), m_payload->filename_base(), false);

  _putchar_method_reference =
      MethodReference::resolve_method(putchar_symbol, "libc", true);
  _getchar_method_reference =
      MethodReference::resolve_method(getchar_symbol, "libc", true);

  _to_char_array_method_reference = MethodReference::resolve_method(
      s_factory::simple_function("toCharArray",
                                 s_factory::char_type()->as_array_type()),
      "java/lang/String", false);

  auto j_str_type = s_factory::type("java/lang/String");
  auto j_main_symbol = s_factory::function("main", s_factory::void_type(),
                                           {j_str_type->as_array_type()});
  auto _j_main_method_reference = MethodReference::resolve_method(
      j_main_symbol, m_payload->filename_base(), true, true);

  m_resolved_method_references.push_back(_init_method_reference);
  m_resolved_method_references.push_back(_clinit_method_reference);
  m_resolved_method_references.push_back(_putchar_method_reference);
  m_resolved_method_references.push_back(_getchar_method_reference);
  m_resolved_method_references.push_back(_to_char_array_method_reference);
  m_resolved_method_references.push_back(_j_main_method_reference);

  for (auto decl : m_payload->lowered_global_declarations()) {
    if (decl->kind() == Binding::BoundNodeKind::FunctionDefinition) {
      auto method = dynamic_cast<Binding::BoundFunctionDefinitionNode *>(decl);
      auto _method_reference = MethodReference::resolve_and_define_method(
          method, m_payload->filename_base(), true);
      m_resolved_method_references.push_back(_method_reference);
    }
  }

  // add to method map
  for (auto method : m_resolved_method_references) {
    add_method_to_map(method);
  }
}

std::string _mangle_name(std::string method_name, std::string owner,
                         Symbols::FunctionSymbol *function) {
  // assume that method_name == function->name();
  std::string param_type_names = "_";
  if (function != nullptr) {
    for (auto p : function->params()) {
      param_type_names += p->type()->jasm_str();
    }
  }
  return owner + "@@" + method_name +
         param_type_names; // <-- I can't see that conflicting with anything.
}

std::string _mangle_name(std::string method_name, std::string owner) {
  // assumes method has no parameters
  return _mangle_name(method_name, owner, nullptr);
}

std::string _mangle_name(MethodReference *method) {
  auto method_name = method->method()->name();
  auto owner_name = method->owner();
  return _mangle_name(method_name, owner_name, method->method());
}

void CodeGenerator::add_method_to_map(MethodReference *method) {
  std::string key = _mangle_name(method);
  m_method_map[key] = method;
}

void CodeGenerator::update_method_in_method_map(MethodReference *new_method) {
  // This method is a carbon copy of above, but the name lessens the
  // cognitive load for me.
  std::string key = _mangle_name(new_method);
  m_method_map[key] = new_method;
}

MethodReference *CodeGenerator::look_up_method(std::string name) {
  return look_up_method(name, m_payload->filename_base());
}

MethodReference *CodeGenerator::look_up_method(std::string name,
                                               std::string owner) {
  std::string key = _mangle_name(name, owner);
  return m_method_map[key]; // maybe handle possible errors at some point?
}

MethodReference *CodeGenerator::look_up_method(Symbols::FunctionSymbol *func) {
  auto name = func->name();
  std::string mangled_key = _mangle_name(name, "[PLACEHOLDER]", func);
  std::string key = mangled_key.substr(mangled_key.find("@@") + 2);
  for (auto kv : m_method_map) {
    std::string mangled_name = kv.first;
    std::string name = mangled_name.substr(mangled_name.find("@@") + 2);
    if (key.compare(name) == 0) {
      return m_method_map[mangled_name];
    }
  }
  return nullptr; // shouldn't happen.
                  // return m_method_map[key];
}

} // namespace CodeGen
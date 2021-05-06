#include "jvm_opcode_info.h"
#include "jvm_opcodes.h"
#include "instruction_arguments.h"
#include <stdexcept>
#include <cassert>

namespace JVMProcessor {
namespace OpCodeInfo {
using namespace JVMProcessor;
bool is_branch(JVMOpCode op_code) {
    switch (op_code) {
        case JVMOpCode::ifeq: [[fallthrough]]; 
        case JVMOpCode::ifne: [[fallthrough]]; 
        case JVMOpCode::iflt: [[fallthrough]]; 
        case JVMOpCode::ifge: [[fallthrough]]; 
        case JVMOpCode::ifgt: [[fallthrough]]; 
        case JVMOpCode::ifle: [[fallthrough]]; 
        case JVMOpCode::if_icmpeq: [[fallthrough]]; 
        case JVMOpCode::if_icmpne: [[fallthrough]]; 
        case JVMOpCode::if_icmplt: [[fallthrough]]; 
        case JVMOpCode::if_icmpge: [[fallthrough]]; 
        case JVMOpCode::if_icmpgt: [[fallthrough]]; 
        case JVMOpCode::if_icmple: [[fallthrough]]; 
        case JVMOpCode::if_acmpeq: [[fallthrough]]; 
        case JVMOpCode::if_acmpne: [[fallthrough]]; 
        case JVMOpCode::_goto: return true;
        default: return false;
    }
}

bool is_conditional_branch(JVMOpCode op_code) {
    switch (op_code) {
        case JVMOpCode::ifeq: [[fallthrough]]; 
        case JVMOpCode::ifne: [[fallthrough]]; 
        case JVMOpCode::iflt: [[fallthrough]]; 
        case JVMOpCode::ifge: [[fallthrough]]; 
        case JVMOpCode::ifgt: [[fallthrough]]; 
        case JVMOpCode::ifle: [[fallthrough]]; 
        case JVMOpCode::if_icmpeq: [[fallthrough]]; 
        case JVMOpCode::if_icmpne: [[fallthrough]]; 
        case JVMOpCode::if_icmplt: [[fallthrough]]; 
        case JVMOpCode::if_icmpge: [[fallthrough]]; 
        case JVMOpCode::if_icmpgt: [[fallthrough]]; 
        case JVMOpCode::if_icmple: [[fallthrough]]; 
        case JVMOpCode::if_acmpeq: [[fallthrough]]; 
        case JVMOpCode::if_acmpne: return true;
        default: return false;
    }
}

bool is_relational_branch(JVMOpCode op_code) {
    switch (op_code) {
        case JVMOpCode::if_icmpeq: [[fallthrough]]; 
        case JVMOpCode::if_icmpne: [[fallthrough]]; 
        case JVMOpCode::if_icmplt: [[fallthrough]]; 
        case JVMOpCode::if_icmpge: [[fallthrough]]; 
        case JVMOpCode::if_icmpgt: [[fallthrough]]; 
        case JVMOpCode::if_icmple: [[fallthrough]]; 
        case JVMOpCode::if_acmpeq: [[fallthrough]]; 
        case JVMOpCode::if_acmpne: return true;
        default: return false;
    }
}

int8_t stack_pop_modifier(JVMOpCode op_code) {
    switch (op_code) {
        case JVMOpCode::nop: [[fallthrough]];
        case JVMOpCode::aconst_null: [[fallthrough]];
        case JVMOpCode::iconst_m1: [[fallthrough]];
        case JVMOpCode::iconst_0: [[fallthrough]];
        case JVMOpCode::iconst_1: [[fallthrough]];
        case JVMOpCode::iconst_2: [[fallthrough]];
        case JVMOpCode::iconst_3: [[fallthrough]];
        case JVMOpCode::iconst_4: [[fallthrough]];
        case JVMOpCode::iconst_5: [[fallthrough]];
        case JVMOpCode::fconst_0: [[fallthrough]];
        case JVMOpCode::fconst_1: [[fallthrough]];
        case JVMOpCode::fconst_2: [[fallthrough]];
        case JVMOpCode::bipush: [[fallthrough]];
        case JVMOpCode::sipush: [[fallthrough]];
        case JVMOpCode::ldc: [[fallthrough]];
        case JVMOpCode::iload: [[fallthrough]];
        case JVMOpCode::fload: [[fallthrough]];
        case JVMOpCode::aload: [[fallthrough]];
        case JVMOpCode::iload_0: [[fallthrough]];
        case JVMOpCode::iload_1: [[fallthrough]];
        case JVMOpCode::iload_2: [[fallthrough]];
        case JVMOpCode::iload_3: [[fallthrough]];
        case JVMOpCode::fload_0: [[fallthrough]];
        case JVMOpCode::fload_1: [[fallthrough]];
        case JVMOpCode::fload_2: [[fallthrough]];
        case JVMOpCode::fload_3: [[fallthrough]];
        case JVMOpCode::aload_0: [[fallthrough]];
        case JVMOpCode::aload_1: [[fallthrough]];
        case JVMOpCode::aload_2: [[fallthrough]];
        case JVMOpCode::aload_3: return 0;

        case JVMOpCode::iaload: [[fallthrough]];
        case JVMOpCode::faload: [[fallthrough]];
        case JVMOpCode::aaload: [[fallthrough]];
        case JVMOpCode::caload: return 2;

        case JVMOpCode::istore: [[fallthrough]];
        case JVMOpCode::fstore: [[fallthrough]];
        case JVMOpCode::astore: [[fallthrough]];
        case JVMOpCode::istore_0: [[fallthrough]];
        case JVMOpCode::istore_1: [[fallthrough]];
        case JVMOpCode::istore_2: [[fallthrough]];
        case JVMOpCode::istore_3: [[fallthrough]];
        case JVMOpCode::fstore_0: [[fallthrough]];
        case JVMOpCode::fstore_1: [[fallthrough]];
        case JVMOpCode::fstore_2: [[fallthrough]];
        case JVMOpCode::fstore_3: [[fallthrough]];
        case JVMOpCode::astore_0: [[fallthrough]];
        case JVMOpCode::astore_1: [[fallthrough]];
        case JVMOpCode::astore_2: [[fallthrough]];
        case JVMOpCode::astore_3: return 1;

        case JVMOpCode::iastore: [[fallthrough]];
        case JVMOpCode::fastore: [[fallthrough]];
        case JVMOpCode::aastore: [[fallthrough]];
        case JVMOpCode::castore: return 3;

        case JVMOpCode::pop: return 1;
        case JVMOpCode::pop2: return 2;
        case JVMOpCode::dup: return 1; // pops off before pushing two back on
        case JVMOpCode::dup2: return 2; // pops two off before pushing them back on twice
        case JVMOpCode::swap: return 2; // pops two off before reversing and pushing them back on
        
        case JVMOpCode::iadd: [[fallthrough]];
        case JVMOpCode::fadd: [[fallthrough]];
        case JVMOpCode::isub: [[fallthrough]];
        case JVMOpCode::fsub: [[fallthrough]];
        case JVMOpCode::imul: [[fallthrough]];
        case JVMOpCode::fmul: [[fallthrough]];
        case JVMOpCode::idiv: [[fallthrough]];
        case JVMOpCode::fdiv: [[fallthrough]];
        case JVMOpCode::irem: [[fallthrough]];
        case JVMOpCode::frem: return 2;

        case JVMOpCode::ineg: [[fallthrough]];
        case JVMOpCode::fneg: return 1;

        case JVMOpCode::iand: [[fallthrough]];
        case JVMOpCode::ior: return 2;

        case JVMOpCode::iinc: return 0;

        case JVMOpCode::i2f: [[fallthrough]];
        case JVMOpCode::f2i: [[fallthrough]];
        case JVMOpCode::i2b: [[fallthrough]];
        case JVMOpCode::i2c: return 1;

        case JVMOpCode::fcmpl: [[fallthrough]];
        case JVMOpCode::fcmpg: return 2;

        case JVMOpCode::ifeq: [[fallthrough]];
        case JVMOpCode::ifne: [[fallthrough]];
        case JVMOpCode::iflt: [[fallthrough]];
        case JVMOpCode::ifge: [[fallthrough]];
        case JVMOpCode::ifgt: [[fallthrough]];
        case JVMOpCode::ifle: return 1;

        case JVMOpCode::if_icmpeq: [[fallthrough]];
        case JVMOpCode::if_icmpne: [[fallthrough]];
        case JVMOpCode::if_icmplt: [[fallthrough]];
        case JVMOpCode::if_icmpge: [[fallthrough]];
        case JVMOpCode::if_icmpgt: [[fallthrough]];
        case JVMOpCode::if_icmple: [[fallthrough]];
        case JVMOpCode::if_acmpeq: [[fallthrough]];
        case JVMOpCode::if_acmpne: return 1;

        case JVMOpCode::_goto: return 0;

        case JVMOpCode::ireturn: [[fallthrough]];
        case JVMOpCode::freturn: [[fallthrough]];
        case JVMOpCode::areturn: [[fallthrough]];
        case JVMOpCode::_return: return -1; // empties the stack.
        
        case JVMOpCode::getstatic: return 0;
        case JVMOpCode::putstatic: return 1;
        case JVMOpCode::getfield: return 1;
        case JVMOpCode::putfield: return 2;

        case JVMOpCode::invokevirtual: [[fallthrough]];
        case JVMOpCode::invokespecial: [[fallthrough]];
        case JVMOpCode::invokestatic: return -1; // can't be arbitrarily long like some others, but since it could be either 0 or 1 we consider it variable
        
        case JVMOpCode::_new: return 0;

        case JVMOpCode::newarray: [[fallthrough]];
        case JVMOpCode::anewarray: return 1; // pops the size of the array before creating it

        case JVMOpCode::arraylength: return 1; // pops ref before pushing length
        default:
            throw new std::runtime_error("Invalid op code value: " + std::to_string(static_cast<int>(op_code)));
    }
}

int8_t stack_push_modifier(JVMOpCode op_code) {
    switch (op_code) {
        case JVMOpCode::nop: [[fallthrough]];
        case JVMOpCode::aconst_null: [[fallthrough]];
        case JVMOpCode::iconst_m1: [[fallthrough]];
        case JVMOpCode::iconst_0: [[fallthrough]];
        case JVMOpCode::iconst_1: [[fallthrough]];
        case JVMOpCode::iconst_2: [[fallthrough]];
        case JVMOpCode::iconst_3: [[fallthrough]];
        case JVMOpCode::iconst_4: [[fallthrough]];
        case JVMOpCode::iconst_5: [[fallthrough]];
        case JVMOpCode::fconst_0: [[fallthrough]];
        case JVMOpCode::fconst_1: [[fallthrough]];
        case JVMOpCode::fconst_2: [[fallthrough]];
        case JVMOpCode::bipush: [[fallthrough]];
        case JVMOpCode::sipush: [[fallthrough]];
        case JVMOpCode::ldc: [[fallthrough]];
        case JVMOpCode::iload: [[fallthrough]];
        case JVMOpCode::fload: [[fallthrough]];
        case JVMOpCode::aload: [[fallthrough]];
        case JVMOpCode::iload_0: [[fallthrough]];
        case JVMOpCode::iload_1: [[fallthrough]];
        case JVMOpCode::iload_2: [[fallthrough]];
        case JVMOpCode::iload_3: [[fallthrough]];
        case JVMOpCode::fload_0: [[fallthrough]];
        case JVMOpCode::fload_1: [[fallthrough]];
        case JVMOpCode::fload_2: [[fallthrough]];
        case JVMOpCode::fload_3: [[fallthrough]];
        case JVMOpCode::aload_0: [[fallthrough]];
        case JVMOpCode::aload_1: [[fallthrough]];
        case JVMOpCode::aload_2: [[fallthrough]];
        case JVMOpCode::aload_3: return 1;

        case JVMOpCode::iaload: [[fallthrough]];
        case JVMOpCode::faload: [[fallthrough]];
        case JVMOpCode::aaload: [[fallthrough]];
        case JVMOpCode::caload: return 1;

        case JVMOpCode::istore: [[fallthrough]];
        case JVMOpCode::fstore: [[fallthrough]];
        case JVMOpCode::astore: [[fallthrough]];
        case JVMOpCode::istore_0: [[fallthrough]];
        case JVMOpCode::istore_1: [[fallthrough]];
        case JVMOpCode::istore_2: [[fallthrough]];
        case JVMOpCode::istore_3: [[fallthrough]];
        case JVMOpCode::fstore_0: [[fallthrough]];
        case JVMOpCode::fstore_1: [[fallthrough]];
        case JVMOpCode::fstore_2: [[fallthrough]];
        case JVMOpCode::fstore_3: [[fallthrough]];
        case JVMOpCode::astore_0: [[fallthrough]];
        case JVMOpCode::astore_1: [[fallthrough]];
        case JVMOpCode::astore_2: [[fallthrough]];
        case JVMOpCode::astore_3: [[fallthrough]];
        case JVMOpCode::iastore: [[fallthrough]];
        case JVMOpCode::fastore: [[fallthrough]];
        case JVMOpCode::aastore: [[fallthrough]];
        case JVMOpCode::castore: [[fallthrough]];
        case JVMOpCode::pop: [[fallthrough]];
        case JVMOpCode::pop2: return 0;

        case JVMOpCode::dup: return 2;
        case JVMOpCode::dup2: return 4;
        case JVMOpCode::swap: return 2;
        
        case JVMOpCode::iadd: [[fallthrough]];
        case JVMOpCode::fadd: [[fallthrough]];
        case JVMOpCode::isub: [[fallthrough]];
        case JVMOpCode::fsub: [[fallthrough]];
        case JVMOpCode::imul: [[fallthrough]];
        case JVMOpCode::fmul: [[fallthrough]];
        case JVMOpCode::idiv: [[fallthrough]];
        case JVMOpCode::fdiv: [[fallthrough]];
        case JVMOpCode::irem: [[fallthrough]];
        case JVMOpCode::frem: [[fallthrough]];
        case JVMOpCode::ineg: [[fallthrough]];
        case JVMOpCode::fneg: [[fallthrough]];
        case JVMOpCode::iand: [[fallthrough]];
        case JVMOpCode::ior: return 1;

        case JVMOpCode::iinc: return 0;

        case JVMOpCode::i2f: [[fallthrough]];
        case JVMOpCode::f2i: [[fallthrough]];
        case JVMOpCode::i2b: [[fallthrough]];
        case JVMOpCode::i2c: [[fallthrough]];
        case JVMOpCode::fcmpl: [[fallthrough]];
        case JVMOpCode::fcmpg: return 1;

        case JVMOpCode::ifeq: [[fallthrough]];
        case JVMOpCode::ifne: [[fallthrough]];
        case JVMOpCode::iflt: [[fallthrough]];
        case JVMOpCode::ifge: [[fallthrough]];
        case JVMOpCode::ifgt: [[fallthrough]];
        case JVMOpCode::ifle: [[fallthrough]];
        case JVMOpCode::if_icmpeq: [[fallthrough]];
        case JVMOpCode::if_icmpne: [[fallthrough]];
        case JVMOpCode::if_icmplt: [[fallthrough]];
        case JVMOpCode::if_icmpge: [[fallthrough]];
        case JVMOpCode::if_icmpgt: [[fallthrough]];
        case JVMOpCode::if_icmple: [[fallthrough]];
        case JVMOpCode::if_acmpeq: [[fallthrough]];
        case JVMOpCode::if_acmpne: [[fallthrough]];
        case JVMOpCode::_goto: [[fallthrough]];
        case JVMOpCode::ireturn: [[fallthrough]];
        case JVMOpCode::freturn: [[fallthrough]];
        case JVMOpCode::areturn: [[fallthrough]];
        case JVMOpCode::_return: return 0; // doesn't push anything
        
        case JVMOpCode::getstatic: return 1;
        case JVMOpCode::putstatic: return 0;
        case JVMOpCode::getfield: return 1;
        case JVMOpCode::putfield: return 0;

        case JVMOpCode::invokevirtual: [[fallthrough]];
        case JVMOpCode::invokespecial: [[fallthrough]];
        case JVMOpCode::invokestatic: return -1; // can't be arbitrarily long like some others, but since it could be either 0 or 1 we consider it variable
        
        case JVMOpCode::_new: return 1;

        case JVMOpCode::newarray: [[fallthrough]];
        case JVMOpCode::anewarray: return 1;

        case JVMOpCode::arraylength: return 1;
        default:
            throw new std::runtime_error("Invalid op code value: " + std::to_string(static_cast<int>(op_code)));
    }
}

bool is_constant_stack_modifier(JVMOpCode op_code) {
    switch (op_code) {
        case JVMOpCode::ireturn: [[fallthrough]];
        case JVMOpCode::freturn: [[fallthrough]];
        case JVMOpCode::areturn: [[fallthrough]];
        case JVMOpCode::_return: [[fallthrough]];
        case JVMOpCode::invokevirtual: [[fallthrough]];
        case JVMOpCode::invokespecial: [[fallthrough]];
        case JVMOpCode::invokestatic: return false;
        default: return true;
    }
}

int net_stack_modification(JVMOpCode op_code) {
    assert((is_constant_stack_modifier(op_code), "Cannot determine net stack modification of variable modifier."));
    return stack_push_modifier(op_code) - stack_pop_modifier(op_code);
}

bool is_control_transfer(JVMOpCode op_code) {
    if (is_branch(op_code)) return true;

    switch (op_code) {
        case JVMOpCode::ireturn: [[fallthrough]];
        case JVMOpCode::freturn: [[fallthrough]];
        case JVMOpCode::areturn: [[fallthrough]];
        case JVMOpCode::_return: return true;
        default: return false;
    }
}

std::ostream& operator<<(std::ostream &strm, const JVMOpCode &op_code) {
    return strm << op_code_name(op_code);
}

std::string op_code_name(JVMOpCode op_code) {
    switch (op_code) {
        case JVMOpCode::nop: return "nop";
        case JVMOpCode::aconst_null: return "aconst_null";
        case JVMOpCode::iconst_m1: return "iconst_m1";
        case JVMOpCode::iconst_0: return "iconst_0";
        case JVMOpCode::iconst_1: return "iconst_1";
        case JVMOpCode::iconst_2: return "iconst_2";
        case JVMOpCode::iconst_3: return "iconst_3";
        case JVMOpCode::iconst_4: return "iconst_4";
        case JVMOpCode::iconst_5: return "iconst_5";
        case JVMOpCode::fconst_0: return "fconst_0";
        case JVMOpCode::fconst_1: return "fconst_1";
        case JVMOpCode::fconst_2: return "fconst_2";
        case JVMOpCode::bipush: return "bipush";
        case JVMOpCode::sipush: return "sipush";
        case JVMOpCode::ldc: return "ldc";
        case JVMOpCode::iload: return "iload";
        case JVMOpCode::fload: return "fload";
        case JVMOpCode::aload: return "aload";
        case JVMOpCode::iload_0: return "iload_0";
        case JVMOpCode::iload_1: return "iload_1";
        case JVMOpCode::iload_2: return "iload_2";
        case JVMOpCode::iload_3: return "iload_3";
        case JVMOpCode::fload_0: return "fload_0";
        case JVMOpCode::fload_1: return "fload_1";
        case JVMOpCode::fload_2: return "fload_2";
        case JVMOpCode::fload_3: return "fload_3";
        case JVMOpCode::aload_0: return "aload_0";
        case JVMOpCode::aload_1: return "aload_1";
        case JVMOpCode::aload_2: return "aload_2";
        case JVMOpCode::aload_3: return "aload_3";
        case JVMOpCode::iaload: return "iaload";
        case JVMOpCode::faload: return "faload";
        case JVMOpCode::aaload: return "aaload";
        case JVMOpCode::caload: return "caload";
        case JVMOpCode::istore: return "istore";
        case JVMOpCode::fstore: return "fstore";
        case JVMOpCode::astore: return "astore";
        case JVMOpCode::istore_0: return "istore_0";
        case JVMOpCode::istore_1: return "istore_1";
        case JVMOpCode::istore_2: return "istore_2";
        case JVMOpCode::istore_3: return "istore_3";
        case JVMOpCode::fstore_0: return "fstore_0";
        case JVMOpCode::fstore_1: return "fstore_1";
        case JVMOpCode::fstore_2: return "fstore_2";
        case JVMOpCode::fstore_3: return "fstore_3";
        case JVMOpCode::astore_0: return "astore_0";
        case JVMOpCode::astore_1: return "astore_1";
        case JVMOpCode::astore_2: return "astore_2";
        case JVMOpCode::astore_3: return "astore_3";
        case JVMOpCode::iastore: return "iastore";
        case JVMOpCode::fastore: return "fastore";
        case JVMOpCode::aastore: return "aastore";
        case JVMOpCode::castore: return "castore";
        case JVMOpCode::pop: return "pop";
        case JVMOpCode::pop2: return "pop2";
        case JVMOpCode::dup: return "dup";
        case JVMOpCode::dup2: return "dup2";
        case JVMOpCode::swap: return "swap";
        case JVMOpCode::iadd: return "iadd";
        case JVMOpCode::fadd: return "fadd";
        case JVMOpCode::isub: return "isub";
        case JVMOpCode::fsub: return "fsub";
        case JVMOpCode::imul: return "imul";
        case JVMOpCode::fmul: return "fmul";
        case JVMOpCode::idiv: return "idiv";
        case JVMOpCode::fdiv: return "fdiv";
        case JVMOpCode::irem: return "irem";
        case JVMOpCode::frem: return "frem";
        case JVMOpCode::ineg: return "ineg";
        case JVMOpCode::fneg: return "fneg";
        case JVMOpCode::iand: return "iand";
        case JVMOpCode::ior: return "ior";
        case JVMOpCode::iinc: return "iinc";
        case JVMOpCode::i2f: return "i2f";
        case JVMOpCode::f2i: return "f2i";
        case JVMOpCode::i2b: return "i2b";
        case JVMOpCode::i2c: return "i2c";
        case JVMOpCode::fcmpl: return "fcmpl";
        case JVMOpCode::fcmpg: return "fcmpg";
        case JVMOpCode::ifeq: return "ifeq";
        case JVMOpCode::ifne: return "ifne";
        case JVMOpCode::iflt: return "iflt";
        case JVMOpCode::ifge: return "ifge";
        case JVMOpCode::ifgt: return "ifgt";
        case JVMOpCode::ifle: return "ifle";
        case JVMOpCode::if_icmpeq: return "if_icmpeq";
        case JVMOpCode::if_icmpne: return "if_icmpne";
        case JVMOpCode::if_icmplt: return "if_icmplt";
        case JVMOpCode::if_icmpge: return "if_icmpge";
        case JVMOpCode::if_icmpgt: return "if_icmpgt";
        case JVMOpCode::if_icmple: return "if_icmple";
        case JVMOpCode::if_acmpeq: return "if_acmpeq";
        case JVMOpCode::if_acmpne: return "if_acmpne";
        case JVMOpCode::_goto: return "goto";
        case JVMOpCode::ireturn: return "ireturn";
        case JVMOpCode::freturn: return "freturn";
        case JVMOpCode::areturn: return "areturn";
        case JVMOpCode::_return: return "return";
        case JVMOpCode::getstatic: return "getstatic";
        case JVMOpCode::putstatic: return "putstatic";
        case JVMOpCode::getfield: return "getfield";
        case JVMOpCode::putfield: return "putfield";
        case JVMOpCode::invokevirtual: return "invokevirtual";
        case JVMOpCode::invokespecial: return "invokespecial";
        case JVMOpCode::invokestatic: return "invokestatic";
        case JVMOpCode::_new: return "new";
        case JVMOpCode::newarray: return "newarray";
        case JVMOpCode::anewarray: return "anewarray";
        case JVMOpCode::arraylength: return "arraylength";
        default:
            throw new std::runtime_error("Invalid op code value: " + std::to_string(static_cast<int>(op_code)));
    }
}


InstructionArgumentKind required_argument_kind(JVMOpCode op_code) {
    switch (op_code) {
        case JVMOpCode::bipush: return InstructionArgumentKind::Char_Constant;
        
        case JVMOpCode::sipush: return InstructionArgumentKind::Integer_Constant; // we'll still emit sipush 

        case JVMOpCode::ldc: return InstructionArgumentKind::Any_Constant;

        case JVMOpCode::iload: [[fallthrough]];
        case JVMOpCode::fload: [[fallthrough]];
        case JVMOpCode::aload: return InstructionArgumentKind::Integer_Constant;

        case JVMOpCode::istore: [[fallthrough]];
        case JVMOpCode::fstore: [[fallthrough]];
        case JVMOpCode::astore: return InstructionArgumentKind::Integer_Constant;

        case JVMOpCode::iinc: return InstructionArgumentKind::Integer_Tuple;

        case JVMOpCode::ifeq: [[fallthrough]];
        case JVMOpCode::ifne: [[fallthrough]];
        case JVMOpCode::iflt: [[fallthrough]];
        case JVMOpCode::ifge: [[fallthrough]];
        case JVMOpCode::ifgt: [[fallthrough]];
        case JVMOpCode::ifle: [[fallthrough]];
        case JVMOpCode::if_icmpeq: [[fallthrough]];
        case JVMOpCode::if_icmpne: [[fallthrough]];
        case JVMOpCode::if_icmplt: [[fallthrough]];
        case JVMOpCode::if_icmpge: [[fallthrough]];
        case JVMOpCode::if_icmpgt: [[fallthrough]];
        case JVMOpCode::if_icmple: [[fallthrough]];
        case JVMOpCode::if_acmpeq: [[fallthrough]];
        case JVMOpCode::if_acmpne: [[fallthrough]];
        case JVMOpCode::_goto: return InstructionArgumentKind::Label;

        case JVMOpCode::getstatic: [[fallthrough]];
        case JVMOpCode::putstatic: [[fallthrough]];
        case JVMOpCode::getfield: [[fallthrough]];
        case JVMOpCode::putfield: return InstructionArgumentKind::Field_Accessor;

        case JVMOpCode::invokevirtual: [[fallthrough]];
        case JVMOpCode::invokespecial: [[fallthrough]];
        case JVMOpCode::invokestatic: return InstructionArgumentKind::Method_Call;

        case JVMOpCode::_new: [[fallthrough]];
        case JVMOpCode::newarray: [[fallthrough]];
        case JVMOpCode::anewarray: return InstructionArgumentKind::Type_Identifier;
        
        default: return InstructionArgumentKind::Empty; 
    }
}

}
}
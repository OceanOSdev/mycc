#include "jvm_opcode_info.h"
#include "jvm_opcodes.h"
#include "instruction_arguments.h"
#include <stdexcept>
#include <cassert>

namespace JVMProcessor {
namespace OpCodeInfo {

bool is_branch(JVMOpCode op_code) {
    switch (op_code) {
        case ifeq: [[fallthrough]]; 
        case ifne: [[fallthrough]]; 
        case iflt: [[fallthrough]]; 
        case ifge: [[fallthrough]]; 
        case ifgt: [[fallthrough]]; 
        case ifle: [[fallthrough]]; 
        case if_icmpeq: [[fallthrough]]; 
        case if_icmpne: [[fallthrough]]; 
        case if_icmplt: [[fallthrough]]; 
        case if_icmpge: [[fallthrough]]; 
        case if_icmpgt: [[fallthrough]]; 
        case if_icmple: [[fallthrough]]; 
        case if_acmpeq: [[fallthrough]]; 
        case if_acmpne: [[fallthrough]]; 
        case _goto: return true;
        default: return false;
    }
}

bool is_conditional_branch(JVMOpCode op_code) {
    switch (op_code) {
        case ifeq: [[fallthrough]]; 
        case ifne: [[fallthrough]]; 
        case iflt: [[fallthrough]]; 
        case ifge: [[fallthrough]]; 
        case ifgt: [[fallthrough]]; 
        case ifle: [[fallthrough]]; 
        case if_icmpeq: [[fallthrough]]; 
        case if_icmpne: [[fallthrough]]; 
        case if_icmplt: [[fallthrough]]; 
        case if_icmpge: [[fallthrough]]; 
        case if_icmpgt: [[fallthrough]]; 
        case if_icmple: [[fallthrough]]; 
        case if_acmpeq: [[fallthrough]]; 
        case if_acmpne: return true;
        default: return false;
    }
}

bool is_relational_branch(JVMOpCode op_code) {
    switch (op_code) {
        case if_icmpeq: [[fallthrough]]; 
        case if_icmpne: [[fallthrough]]; 
        case if_icmplt: [[fallthrough]]; 
        case if_icmpge: [[fallthrough]]; 
        case if_icmpgt: [[fallthrough]]; 
        case if_icmple: [[fallthrough]]; 
        case if_acmpeq: [[fallthrough]]; 
        case if_acmpne: return true;
        default: return false;
    }
}

int8_t stack_pop_modifier(JVMOpCode op_code) {
    switch (op_code) {
        case nop: [[fallthrough]];
        case aconst_null: [[fallthrough]];
        case iconst_m1: [[fallthrough]];
        case iconst_0: [[fallthrough]];
        case iconst_1: [[fallthrough]];
        case iconst_2: [[fallthrough]];
        case iconst_3: [[fallthrough]];
        case iconst_4: [[fallthrough]];
        case iconst_5: [[fallthrough]];
        case fconst_0: [[fallthrough]];
        case fconst_1: [[fallthrough]];
        case fconst_2: [[fallthrough]];
        case bipush: [[fallthrough]];
        case sipush: [[fallthrough]];
        case ldc: [[fallthrough]];
        case iload: [[fallthrough]];
        case fload: [[fallthrough]];
        case aload: [[fallthrough]];
        case iload_0: [[fallthrough]];
        case iload_1: [[fallthrough]];
        case iload_2: [[fallthrough]];
        case iload_3: [[fallthrough]];
        case fload_0: [[fallthrough]];
        case fload_1: [[fallthrough]];
        case fload_2: [[fallthrough]];
        case fload_3: [[fallthrough]];
        case aload_0: [[fallthrough]];
        case aload_1: [[fallthrough]];
        case aload_2: [[fallthrough]];
        case aload_3: return 0;

        case iaload: [[fallthrough]];
        case faload: [[fallthrough]];
        case aaload: [[fallthrough]];
        case caload: return 2;

        case istore: [[fallthrough]];
        case fstore: [[fallthrough]];
        case astore: [[fallthrough]];
        case istore_0: [[fallthrough]];
        case istore_1: [[fallthrough]];
        case istore_2: [[fallthrough]];
        case istore_3: [[fallthrough]];
        case fstore_0: [[fallthrough]];
        case fstore_1: [[fallthrough]];
        case fstore_2: [[fallthrough]];
        case fstore_3: [[fallthrough]];
        case astore_0: [[fallthrough]];
        case astore_1: [[fallthrough]];
        case astore_2: [[fallthrough]];
        case astore_3: return 1;

        case iastore: [[fallthrough]];
        case fastore: [[fallthrough]];
        case aastore: [[fallthrough]];
        case castore: return 3;

        case pop: return 1;
        case pop2: return 2;
        case dup: return 1; // pops off before pushing two back on
        case dup2: return 2; // pops two off before pushing them back on twice
        case swap: return 2; // pops two off before reversing and pushing them back on
        
        case iadd: [[fallthrough]];
        case fadd: [[fallthrough]];
        case isub: [[fallthrough]];
        case fsub: [[fallthrough]];
        case imul: [[fallthrough]];
        case fmul: [[fallthrough]];
        case idiv: [[fallthrough]];
        case fdiv: [[fallthrough]];
        case irem: [[fallthrough]];
        case frem: return 2;

        case ineg: [[fallthrough]];
        case fneg: return 1;

        case iand: [[fallthrough]];
        case ior: return 2;

        case iinc: return 0;

        case i2f: [[fallthrough]];
        case f2i: [[fallthrough]];
        case i2b: [[fallthrough]];
        case i2c: return 1;

        case fcmpl: [[fallthrough]];
        case fcmpg: return 2;

        case ifeq: [[fallthrough]];
        case ifne: [[fallthrough]];
        case iflt: [[fallthrough]];
        case ifge: [[fallthrough]];
        case ifgt: [[fallthrough]];
        case ifle: return 1;

        case if_icmpeq: [[fallthrough]];
        case if_icmpne: [[fallthrough]];
        case if_icmplt: [[fallthrough]];
        case if_icmpge: [[fallthrough]];
        case if_icmpgt: [[fallthrough]];
        case if_icmple: [[fallthrough]];
        case if_acmpeq: [[fallthrough]];
        case if_acmpne: return 1;

        case _goto: return 0;

        case ireturn: [[fallthrough]];
        case freturn: [[fallthrough]];
        case areturn: [[fallthrough]];
        case _return: return -1; // empties the stack.
        
        case getstatic: return 0;
        case putstatic: return 1;
        case getfield: return 1;
        case putfield: return 2;

        case invokevirtual: [[fallthrough]];
        case invokespecial: [[fallthrough]];
        case invokestatic: return -1; // can't be arbitrarily long like some others, but since it could be either 0 or 1 we consider it variable
        
        case _new: return 0;

        case newarray: [[fallthrough]];
        case anewarray: return 1; // pops the size of the array before creating it

        case arraylength: return 1; // pops ref before pushing length
        default:
            throw new std::runtime_error("Invalid op code value: " + std::to_string(op_code));
    }
}

int8_t stack_push_modifier(JVMOpCode op_code) {
    switch (op_code) {
        case nop: [[fallthrough]];
        case aconst_null: [[fallthrough]];
        case iconst_m1: [[fallthrough]];
        case iconst_0: [[fallthrough]];
        case iconst_1: [[fallthrough]];
        case iconst_2: [[fallthrough]];
        case iconst_3: [[fallthrough]];
        case iconst_4: [[fallthrough]];
        case iconst_5: [[fallthrough]];
        case fconst_0: [[fallthrough]];
        case fconst_1: [[fallthrough]];
        case fconst_2: [[fallthrough]];
        case bipush: [[fallthrough]];
        case sipush: [[fallthrough]];
        case ldc: [[fallthrough]];
        case iload: [[fallthrough]];
        case fload: [[fallthrough]];
        case aload: [[fallthrough]];
        case iload_0: [[fallthrough]];
        case iload_1: [[fallthrough]];
        case iload_2: [[fallthrough]];
        case iload_3: [[fallthrough]];
        case fload_0: [[fallthrough]];
        case fload_1: [[fallthrough]];
        case fload_2: [[fallthrough]];
        case fload_3: [[fallthrough]];
        case aload_0: [[fallthrough]];
        case aload_1: [[fallthrough]];
        case aload_2: [[fallthrough]];
        case aload_3: return 1;

        case iaload: [[fallthrough]];
        case faload: [[fallthrough]];
        case aaload: [[fallthrough]];
        case caload: return 1;

        case istore: [[fallthrough]];
        case fstore: [[fallthrough]];
        case astore: [[fallthrough]];
        case istore_0: [[fallthrough]];
        case istore_1: [[fallthrough]];
        case istore_2: [[fallthrough]];
        case istore_3: [[fallthrough]];
        case fstore_0: [[fallthrough]];
        case fstore_1: [[fallthrough]];
        case fstore_2: [[fallthrough]];
        case fstore_3: [[fallthrough]];
        case astore_0: [[fallthrough]];
        case astore_1: [[fallthrough]];
        case astore_2: [[fallthrough]];
        case astore_3: [[fallthrough]];
        case iastore: [[fallthrough]];
        case fastore: [[fallthrough]];
        case aastore: [[fallthrough]];
        case castore: [[fallthrough]];
        case pop: [[fallthrough]];
        case pop2: return 0;

        case dup: return 2;
        case dup2: return 4;
        case swap: return 2;
        
        case iadd: [[fallthrough]];
        case fadd: [[fallthrough]];
        case isub: [[fallthrough]];
        case fsub: [[fallthrough]];
        case imul: [[fallthrough]];
        case fmul: [[fallthrough]];
        case idiv: [[fallthrough]];
        case fdiv: [[fallthrough]];
        case irem: [[fallthrough]];
        case frem: [[fallthrough]];
        case ineg: [[fallthrough]];
        case fneg: [[fallthrough]];
        case iand: [[fallthrough]];
        case ior: return 1;

        case iinc: return 0;

        case i2f: [[fallthrough]];
        case f2i: [[fallthrough]];
        case i2b: [[fallthrough]];
        case i2c: [[fallthrough]];
        case fcmpl: [[fallthrough]];
        case fcmpg: return 1;

        case ifeq: [[fallthrough]];
        case ifne: [[fallthrough]];
        case iflt: [[fallthrough]];
        case ifge: [[fallthrough]];
        case ifgt: [[fallthrough]];
        case ifle: [[fallthrough]];
        case if_icmpeq: [[fallthrough]];
        case if_icmpne: [[fallthrough]];
        case if_icmplt: [[fallthrough]];
        case if_icmpge: [[fallthrough]];
        case if_icmpgt: [[fallthrough]];
        case if_icmple: [[fallthrough]];
        case if_acmpeq: [[fallthrough]];
        case if_acmpne: [[fallthrough]];
        case _goto: [[fallthrough]];
        case ireturn: [[fallthrough]];
        case freturn: [[fallthrough]];
        case areturn: [[fallthrough]];
        case _return: return 0; // doesn't push anything
        
        case getstatic: return 1;
        case putstatic: return 0;
        case getfield: return 1;
        case putfield: return 0;

        case invokevirtual: [[fallthrough]];
        case invokespecial: [[fallthrough]];
        case invokestatic: return -1; // can't be arbitrarily long like some others, but since it could be either 0 or 1 we consider it variable
        
        case _new: return 1;

        case newarray: [[fallthrough]];
        case anewarray: return 1;

        case arraylength: return 1;
        default:
            throw new std::runtime_error("Invalid op code value: " + std::to_string(op_code));
    }
}

bool is_constant_stack_modifier(JVMOpCode op_code) {
    switch (op_code) {
        case ireturn: [[fallthrough]];
        case freturn: [[fallthrough]];
        case areturn: [[fallthrough]];
        case _return: [[fallthrough]];
        case invokevirtual: [[fallthrough]];
        case invokespecial: [[fallthrough]];
        case invokestatic: return false;
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
        case ireturn: [[fallthrough]];
        case freturn: [[fallthrough]];
        case areturn: [[fallthrough]];
        case _return: return true;
        default: return false;
    }
}

std::ostream& operator<<(std::ostream &strm, const JVMOpCode &op_code) {
    return strm << op_code_name(op_code);
}

std::string op_code_name(JVMOpCode op_code) {
    switch (op_code) {
        case nop: return "nop";
        case aconst_null: return "aconst_null";
        case iconst_m1: return "iconst_m1";
        case iconst_0: return "iconst_0";
        case iconst_1: return "iconst_1";
        case iconst_2: return "iconst_2";
        case iconst_3: return "iconst_3";
        case iconst_4: return "iconst_4";
        case iconst_5: return "iconst_5";
        case fconst_0: return "fconst_0";
        case fconst_1: return "fconst_1";
        case fconst_2: return "fconst_2";
        case bipush: return "bipush";
        case sipush: return "sipush";
        case ldc: return "ldc";
        case iload: return "iload";
        case fload: return "fload";
        case aload: return "aload";
        case iload_0: return "iload_0";
        case iload_1: return "iload_1";
        case iload_2: return "iload_2";
        case iload_3: return "iload_3";
        case fload_0: return "fload_0";
        case fload_1: return "fload_1";
        case fload_2: return "fload_2";
        case fload_3: return "fload_3";
        case aload_0: return "aload_0";
        case aload_1: return "aload_1";
        case aload_2: return "aload_2";
        case aload_3: return "aload_3";
        case iaload: return "iaload";
        case faload: return "faload";
        case aaload: return "aaload";
        case caload: return "caload";
        case istore: return "istore";
        case fstore: return "fstore";
        case astore: return "astore";
        case istore_0: return "istore_0";
        case istore_1: return "istore_1";
        case istore_2: return "istore_2";
        case istore_3: return "istore_3";
        case fstore_0: return "fstore_0";
        case fstore_1: return "fstore_1";
        case fstore_2: return "fstore_2";
        case fstore_3: return "fstore_3";
        case astore_0: return "astore_0";
        case astore_1: return "astore_1";
        case astore_2: return "astore_2";
        case astore_3: return "astore_3";
        case iastore: return "iastore";
        case fastore: return "fastore";
        case aastore: return "aastore";
        case castore: return "castore";
        case pop: return "pop";
        case pop2: return "pop2";
        case dup: return "dup";
        case dup2: return "dup2";
        case swap: return "swap";
        case iadd: return "iadd";
        case fadd: return "fadd";
        case isub: return "isub";
        case fsub: return "fsub";
        case imul: return "imul";
        case fmul: return "fmul";
        case idiv: return "idiv";
        case fdiv: return "fdiv";
        case irem: return "irem";
        case frem: return "frem";
        case ineg: return "ineg";
        case fneg: return "fneg";
        case iand: return "iand";
        case ior: return "ior";
        case iinc: return "iinc";
        case i2f: return "i2f";
        case f2i: return "f2i";
        case i2b: return "i2b";
        case i2c: return "i2c";
        case fcmpl: return "fcmpl";
        case fcmpg: return "fcmpg";
        case ifeq: return "ifeq";
        case ifne: return "ifne";
        case iflt: return "iflt";
        case ifge: return "ifge";
        case ifgt: return "ifgt";
        case ifle: return "ifle";
        case if_icmpeq: return "if_icmpeq";
        case if_icmpne: return "if_icmpne";
        case if_icmplt: return "if_icmplt";
        case if_icmpge: return "if_icmpge";
        case if_icmpgt: return "if_icmpgt";
        case if_icmple: return "if_icmple";
        case if_acmpeq: return "if_acmpeq";
        case if_acmpne: return "if_acmpne";
        case _goto: return "goto";
        case ireturn: return "ireturn";
        case freturn: return "freturn";
        case areturn: return "areturn";
        case _return: return "return";
        case getstatic: return "getstatic";
        case putstatic: return "putstatic";
        case getfield: return "getfield";
        case putfield: return "putfield";
        case invokevirtual: return "invokevirtual";
        case invokespecial: return "invokespecial";
        case invokestatic: return "invokestatic";
        case _new: return "new";
        case newarray: return "newarray";
        case anewarray: return "anewarray";
        case arraylength: return "arraylength";
        default:
            throw new std::runtime_error("Invalid op code value: " + std::to_string(op_code));
    }
}


InstructionArgumentKind required_argument_kind(JVMOpCode op_code) {
    switch (op_code) {
        case bipush: return InstructionArgumentKind::Char_Constant;
        
        case sipush: return InstructionArgumentKind::Integer_Constant; // we'll still emit sipush 

        case ldc: return InstructionArgumentKind::Any_Constant;

        case iload: [[fallthrough]];
        case fload: [[fallthrough]];
        case aload: return InstructionArgumentKind::Integer_Constant;

        case istore: [[fallthrough]];
        case fstore: [[fallthrough]];
        case astore: return InstructionArgumentKind::Integer_Constant;

        case iinc: return InstructionArgumentKind::Integer_Tuple;

        case ifeq: [[fallthrough]];
        case ifne: [[fallthrough]];
        case iflt: [[fallthrough]];
        case ifge: [[fallthrough]];
        case ifgt: [[fallthrough]];
        case ifle: [[fallthrough]];
        case if_icmpeq: [[fallthrough]];
        case if_icmpne: [[fallthrough]];
        case if_icmplt: [[fallthrough]];
        case if_icmpge: [[fallthrough]];
        case if_icmpgt: [[fallthrough]];
        case if_icmple: [[fallthrough]];
        case if_acmpeq: [[fallthrough]];
        case if_acmpne: [[fallthrough]];
        case _goto: return InstructionArgumentKind::Label;

        case getstatic: [[fallthrough]];
        case putstatic: [[fallthrough]];
        case getfield: [[fallthrough]];
        case putfield: return InstructionArgumentKind::Field_Accessor;

        case invokevirtual: [[fallthrough]];
        case invokespecial: [[fallthrough]];
        case invokestatic: return InstructionArgumentKind::Method_Call;

        case _new: [[fallthrough]];
        case newarray: [[fallthrough]];
        case anewarray: return InstructionArgumentKind::Type_Identifier;
        
        default: return InstructionArgumentKind::Empty; 
    }
}

}
}
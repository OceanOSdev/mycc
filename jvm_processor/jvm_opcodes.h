#pragma once
#ifndef JVM_OPCODES_H
#define JVM_OPCODES_H

#include <cstdint>

namespace JVMProcessor {
/**
 * @brief An enum containing the relavent Java assembly opcodes, named by their mnemonics.
 * 
 * The enum value should correspond to the actual opcode value.
 */
enum JVMOpCode : uint8_t {
    nop = 0x00,
    aconst_null = 0x01,
    iconst_m1 = 0x02,
    iconst_0 = 0x03,
    iconst_1 = 0x04,
    iconst_2 = 0x05,
    iconst_3 = 0x06,
    iconst_4 = 0x07,
    iconst_5 = 0x08,
    fconst_0 = 0x0b,
    fconst_1 = 0x0c,
    fconst_2 = 0x0d,

    bipush = 0x10,
    ldc = 0x12,
    
    iload = 0x15,
    fload = 0x17,
    aload = 0x19,
    iload_0 = 0x1a,
    iload_1 = 0x1b,
    iload_2 = 0x1c,
    iload_3 = 0x1d,
    fload_0 = 0x22,
    fload_1 = 0x23,
    fload_2 = 0x24,
    fload_3 = 0x25,
    aload_0 = 0x2a,
    aload_1 = 0x2b,
    aload_2 = 0x2c,
    aload_3 = 0x2d,
    iaload = 0x2e,
    faload = 0x30,
    aaload = 0x32,
    caload = 0x34,

    istore = 0x36,
    fstore = 0x38,
    astore = 0x3a,
    istore_0 = 0x3b,
    istore_1 = 0x3c,
    istore_2 = 0x3d,
    istore_3 = 0x3e,
    fstore_0 = 0x43,
    fstore_1 = 0x44,
    fstore_2 = 0x45,
    fstore_3 = 0x46,
    astore_0 = 0x3b,
    astore_1 = 0x3c,
    astore_2 = 0x3d,
    astore_3 = 0x3e,
    iastore = 0x4f,
    fastore = 0x51,
    aastore = 0x53,
    castore = 0x55,

    pop = 0x57,
    pop2 = 0x58,
    dup = 0x59,
    dup2 = 0x5c,
    swap = 0x5f,

    iadd = 0x60,
    fadd = 0x62,
    isub = 0x64,
    fsub = 0x66,
    imul = 0x68,
    fmul = 0x6a,
    idiv = 0x6c,
    fdiv = 0x6e,
    irem = 0x70,
    frem = 0x72,
    ineg = 0x74,
    fneg = 0x76

};

}

#endif
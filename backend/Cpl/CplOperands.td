//===- CplOperands.td - Cpl immediate processing -*- tablegen -*-----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illnois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Immediate operands.
def s32ImmPred : PatLeaf<(i32 imm), [{
  int64_t v = (int64_t) N->getSExtValue();
  return isInt<32>(v);
}]>;

def s12ImmPred : PatLeaf<(i32 imm), [{
  int64_t v = (int64_t) N->getSExtValue();
  return isInt<12>(v);
}]>;

def s8ImmPred : PatLeaf<(i32 imm), [{
  int64_t v = (int64_t) N->getSExtValue();
  return isInt<8>(v);
}]>;

def jmptarget : Operand<OtherVT>;

def mem : Operand<i32> {
  let MIOperandInfo = (ops IntRegs, s32ImmPred);
}

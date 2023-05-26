//===-- CplRegisterInfo.cpp - Cpl Register Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Cpl implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#include "CplRegisterInfo.h"
#include "Cpl.h"
#include "CplMachineFunctionInfo.h"
#include "CplSubtarget.h"
#include "CplTargetMachine.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

#define GET_REGINFO_TARGET_DESC
#include "CplGenRegisterInfo.inc"

using namespace llvm;

CplRegisterInfo::CplRegisterInfo(CplSubtarget &st)
  : CplGenRegisterInfo(Cpl::R14, 0, 0, Cpl::R15),
    Subtarget(st) {
}

const MCPhysReg *CplRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF)
  const {
  return CSR_Cpl_SaveList;
}

const uint32_t *CplRegisterInfo::getCallPreservedMask(CallingConv::ID) const {
  return CSR_Cpl_RegMask;
}

BitVector CplRegisterInfo::getReservedRegs(const MachineFunction &MF)
  const {
  BitVector Reserved(getNumRegs());
  Reserved.set(Cpl::R13);
  Reserved.set(Cpl::R15);
  return Reserved;
}

void CplRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                              int SPAdj, unsigned FIOperandNum,
                                              RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");
  MachineInstr &MI = *II;
  unsigned i = 0;
  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() &&
      "Instr doesn't have FrameIndex operand!");
  }
  int FrameIndex = MI.getOperand(i).getIndex();
  MachineFunction &MF = *MI.getParent()->getParent();
  uint64_t stackSize = MF.getFrameInfo()->getStackSize();
  int64_t spOffset = MF.getFrameInfo()->getObjectOffset(FrameIndex);
  int64_t Offset = spOffset + (int64_t) stackSize;

  Offset += MI.getOperand(i + 1).getImm();
  if (!MI.isDebugValue() && !isInt<12>(Offset)) {
    assert("(!MI.isDebugValue() && !isInt<12>(Offset))");
  }

  unsigned FrameReg = getStackRegister();

  MI.getOperand(i).ChangeToRegister(FrameReg, false);
  MI.getOperand(i + 1).ChangeToImmediate(Offset);
}

unsigned CplRegisterInfo::getFrameRegister(const MachineFunction
                                               &MF) const {
  return Cpl::R13;
}

unsigned CplRegisterInfo::getFrameRegister() const {
  return Cpl::R13;
}

unsigned CplRegisterInfo::getStackRegister() const {
  return Cpl::R13;
}

unsigned CplRegisterInfo::getLinkRegister() const {
  return Cpl::R14;
}

// Functie care returneaza registrul R15 (PC)
unsigned CplRegisterInfo::getPcRegister() const {
  return Cpl::R15;
}
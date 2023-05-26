//===-- CplFrameLowering.cpp - Define frame lowering ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//
//===----------------------------------------------------------------------===//

#include "CplFrameLowering.h"
#include "Cpl.h"
#include "CplInstrInfo.h"
#include "CplMachineFunctionInfo.h"
#include "CplRegisterInfo.h"
#include "CplSubtarget.h"
#include "CplTargetMachine.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

/// Adjust SP by Amount bytes.
static void adjustStackPtr(const CplInstrInfo *CplII, unsigned SP,
  int64_t Amount, MachineBasicBlock &MBB, MachineBasicBlock::iterator I) {

  DebugLoc DL = I != MBB.end() ? I->getDebugLoc() : DebugLoc();

  // TODO 4: Add instruction(s) for stack pointer adjustment by Amount.
  // Adaug la SP , Ammount pentru a stii cat spatiu sa aloc pe stiva.
  BuildMI(MBB, I, DL, CplII->get(Cpl::ADDI), SP).addReg(SP).addImm(Amount);
}

void CplFrameLowering::emitPrologue(MachineFunction &MF) const {
  MachineBasicBlock &MBB = MF.front();
  MachineFrameInfo *MFI = MF.getFrameInfo();
  const CplRegisterInfo *CplRI = static_cast<const CplRegisterInfo *>(
    MF.getSubtarget().getRegisterInfo());

  const TargetSubtargetInfo &STI = MF.getSubtarget();
  const CplInstrInfo *TII = static_cast<const CplInstrInfo *>(
    STI.getInstrInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  unsigned SP = CplRI->getStackRegister();

  // First, compute final stack size.
  uint64_t StackSize = MFI->getStackSize();

  // No need to allocate space on the stack.
  if (StackSize == 0 && !MFI->adjustsStack()) {
    return;
  }

  // Adjust stack.
  adjustStackPtr(TII, SP, -StackSize, MBB, MBBI);
}

void CplFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo *MFI = MF.getFrameInfo();

  const CplInstrInfo *TII = static_cast<const CplInstrInfo*>(
    MF.getSubtarget().getInstrInfo());

  DebugLoc dl = MBBI->getDebugLoc();
  const CplRegisterInfo *CplRI = static_cast<const CplRegisterInfo *>(
    MF.getSubtarget().getRegisterInfo());
  unsigned SP = CplRI->getStackRegister();

  // Get the number of bytes from FrameInfo.
  uint64_t StackSize = MFI->getStackSize();

  if (!StackSize)
    return;

  // Adjust stack.
  adjustStackPtr(TII, SP, StackSize, MBB, MBBI);
}

bool CplFrameLowering::hasFP(const MachineFunction &MF) const {
  return false;
}

void CplFrameLowering::
eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator I) const {
  MBB.erase(I);
}

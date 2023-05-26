//===-- CplInstrInfo.cpp - Cpl Instruction Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Cpl implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "CplInstrInfo.h"
#include "Cpl.h"
#include "CplRegisterInfo.h"
#include "CplSubtarget.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/DFAPacketizer.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "Cpl-instrinfo"

#define GET_INSTRINFO_CTOR_DTOR
#define GET_INSTRMAP_INFO
#include "CplGenInstrInfo.inc"
#include "CplGenDFAPacketizer.inc"

// Pin the vtable to this file.
void CplInstrInfo::anchor() {}

CplInstrInfo::CplInstrInfo(CplSubtarget &ST)
  : CplGenInstrInfo(Cpl::ADJCALLSTACKDOWN, Cpl::ADJCALLSTACKUP),
    RI(ST), Subtarget(ST) {
}

void CplInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator I, DebugLoc DL,
                                 unsigned DestReg, unsigned SrcReg,
                                 bool KillSrc) const {
  if (Cpl::IntRegsRegClass.contains(SrcReg, DestReg)) {
    // TODO 4: Add instruction for transfer between physical registers.
    // Copiez valoarea din registrul sursa in registrul destinatie.
    BuildMI(MBB, I, DL, get(Cpl::MOVR), DestReg).addReg(SrcReg);
  }
}

static MachineMemOperand *GetMemOperand(MachineBasicBlock &MBB, int FI,
                                        unsigned Flag) {
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = *MF.getFrameInfo();
  unsigned Align = MFI.getObjectAlignment(FI);

  return MF.getMachineMemOperand(MachinePointerInfo::getFixedStack(FI), Flag,
    MFI.getObjectSize(FI), Align);
}

void CplInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator I,
                                       unsigned SrcReg, bool isKill, int FI,
                                       const TargetRegisterClass *RC,
                                       const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (I != MBB.end()) DL = I->getDebugLoc();
  MachineMemOperand *MMO = GetMemOperand(MBB, FI, MachineMemOperand::MOStore);

  unsigned Opc = Cpl::STR;
  assert(Opc && "Register class not handled!");
  BuildMI(MBB, I, DL, get(Opc)).addReg(SrcReg, getKillRegState(isKill))
    .addFrameIndex(FI).addImm(0).addMemOperand(MMO);
}


void CplInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator I,
                                        unsigned DestReg, int FI,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (I != MBB.end()) DL = I->getDebugLoc();
  MachineMemOperand *MMO = GetMemOperand(MBB, FI, MachineMemOperand::MOLoad);
  unsigned Opc = Cpl::LDR;

  assert(Opc && "Register class not handled!");
  BuildMI(MBB, I, DL, get(Opc), DestReg).addFrameIndex(FI).addImm(0)
    .addMemOperand(MMO);
}

bool CplInstrInfo::
expandPostRAPseudo(MachineBasicBlock::iterator MI) const {

  MachineBasicBlock &MBB = *MI->getParent();
  DebugLoc DL = MI->getDebugLoc();

  if (MI->getOpcode() == Cpl::MOV_I32) {
    MachineOperand &MS = MI->getOperand(1);
    if (MS.isImm()) {
      int64_t Imm = MI->getOperand(1).getImm();
      if (isInt<8>(Imm)) {
        assert(MI->getOperand(0).isReg());
        BuildMI(MBB, MI, DL, get(Cpl::MOVI), MI->getOperand(0).getReg())
          .addImm(Imm);
        MI->eraseFromParent();
        return true;
      }
    }
  }

  if (MI->getOpcode() == Cpl::RET) {
    // TODO 5: Replace RET pseudo instruction with ORR_RET.
    // Transfer LR in PC cu ajutorul operatiei OR.
    BuildMI(MBB, MI, DL, get(Cpl::ORR_RET), RI.getPcRegister()).addReg(RI.getLinkRegister()).addReg(RI.getLinkRegister());
    MI->eraseFromParent();
    return true;
  }

  if (MI->getOpcode() == Cpl::CALLR) {
    // TODO 5: Replace CALLR pseudo instruction with ORR_CALL.
    // Salvez PC-ul in LR, iar apoi rezultatul operatiei OR o depun in registrul PC.
    BuildMI(MBB, MI, DL, get(Cpl::MOVR), RI.getLinkRegister()).addReg(RI.getPcRegister());
    BuildMI(MBB, MI, DL, get(Cpl::ORR_CALL), RI.getPcRegister()).addReg(MI->getOperand(0).getReg()).addReg(MI->getOperand(0).getReg());
    MI->eraseFromParent();
    return true;
  }

  // TODO 6: Implement lowering of compare results with two transfers (at least
  // one should be conditional).
  unsigned Op = 0;
  if (Op) {
    assert(MI->getOperand(0).isReg());
    assert(MI->getOperand(1).isReg());
    assert(MI->getOperand(2).isReg());

    return true;
  }

  if (MI->getOpcode() == Cpl::FRAME)
  {
    assert(MI->getOperand(0).isReg());
    assert(MI->getOperand(2).isImm());
    BuildMI(MBB, MI, DL, get(Cpl::ADDI), MI->getOperand(0).getReg())
      .addReg(RI.getStackRegister())
      .addImm(MI->getOperand(2).getImm());
    MI->eraseFromParent();
    return true;
  }

  return false;
}

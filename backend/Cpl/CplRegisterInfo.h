//==- CplRegisterInfo.h - Cpl Register Information Impl ----------*- C++ -*-==//
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

#ifndef LLVM_LIB_TARGET_Cpl_CplREGISTERINFO_H
#define LLVM_LIB_TARGET_Cpl_CplREGISTERINFO_H

#include "llvm/MC/MachineLocation.h"
#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "CplGenRegisterInfo.inc"

namespace llvm {

class CplSubtarget;
class CplInstrInfo;
class Type;

struct CplRegisterInfo : public CplGenRegisterInfo {
  CplSubtarget &Subtarget;

  CplRegisterInfo(CplSubtarget &st);

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF = nullptr)
    const override;

  const uint32_t *getCallPreservedMask(CallingConv::ID) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator II,
                           int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  unsigned getFrameRegister(const MachineFunction  &MF) const;
  unsigned getFrameRegister() const;
  unsigned getStackRegister() const;
  unsigned getLinkRegister() const;
  unsigned getPcRegister() const;
};

} // end namespace llvm

#endif

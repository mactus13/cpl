//===- CplInstrInfo.h - Cpl Instruction Information -------------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_Cpl_CplINSTRINFO_H
#define LLVM_LIB_TARGET_Cpl_CplINSTRINFO_H

#include "CplRegisterInfo.h"
#include "MCTargetDesc/CplBaseInfo.h"

#include "llvm/CodeGen/MachineBranchProbabilityInfo.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "CplGenInstrInfo.inc"

namespace llvm {

struct EVT;

class CplInstrInfo : public CplGenInstrInfo {
  virtual void anchor();
  const CplRegisterInfo RI;
  const CplSubtarget &Subtarget;
  typedef unsigned Opcode_t;

public:
  explicit CplInstrInfo(CplSubtarget &ST);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  const CplRegisterInfo &getRegisterInfo() const { return RI; }

  void copyPhysReg(MachineBasicBlock &MBB,
                   MachineBasicBlock::iterator I, DebugLoc DL,
                   unsigned DestReg, unsigned SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI,
                           unsigned SrcReg, bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI,
                            unsigned DestReg, int FrameIndex,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;

  bool expandPostRAPseudo(MachineBasicBlock::iterator MI) const override;

};

}

#endif

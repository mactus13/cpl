//=- CplFrameLowering.h - Define frame lowering for Cpl ----------*- C++ -*--=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_CplFRAMELOWERING_H
#define LLVM_LIB_TARGET_Cpl_CplFRAMELOWERING_H

#include "Cpl.h"

#include "llvm/Target/TargetFrameLowering.h"

namespace llvm {

class CplFrameLowering : public TargetFrameLowering {

public:
  explicit CplFrameLowering() : TargetFrameLowering(StackGrowsDown, 4, 0) {}

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;

  void
  eliminateCallFramePseudoInstr(MachineFunction &MF,
                                MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;
};

} // End llvm namespace

#endif

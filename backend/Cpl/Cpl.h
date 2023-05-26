//=-- Cpl.h - Top-level interface for Cpl representation ----------*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// Cpl back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_Cpl_H
#define LLVM_LIB_TARGET_Cpl_Cpl_H

#include "MCTargetDesc/CplMCTargetDesc.h"

#include "llvm/Target/TargetLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class FunctionPass;
  class ModulePass;
  class TargetMachine;
  class MachineInstr;
  class CplMCInst;
  class CplAsmPrinter;
  class CplTargetMachine;
  class raw_ostream;

  FunctionPass *createCplISelDag(CplTargetMachine &TM,
                                     CodeGenOpt::Level OptLevel);

  void CplLowerToMC(const MachineInstr *MI, CplMCInst &MCI,
                        CplAsmPrinter &AP);
} // end namespace llvm;

#endif

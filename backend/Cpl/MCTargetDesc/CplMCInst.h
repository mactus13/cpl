//===- CplMCInst.h - Cpl sub-class of MCInst ------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class extends MCInst to allow some VLIW annotations.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_MCTARGETDESC_CplMCINST_H
#define LLVM_LIB_TARGET_Cpl_MCTARGETDESC_CplMCINST_H

#include "CplTargetMachine.h"
#include "llvm/MC/MCInst.h"
#include <memory>

extern "C" void LLVMInitializeCplTargetMC();
namespace llvm {
class MCOperand;

class CplMCInst : public MCInst {
  friend void ::LLVMInitializeCplTargetMC();
  // Used to access TSFlags
  static std::unique_ptr <MCInstrInfo const> MCII;

public:
  explicit CplMCInst();
  CplMCInst(const MCInstrDesc &mcid);
  
};
}

#endif

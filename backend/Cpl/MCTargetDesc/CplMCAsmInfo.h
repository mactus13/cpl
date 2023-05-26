//===-- CplTargetAsmInfo.h - Cpl asm properties ----------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the CplMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_MCTARGETDESC_CplMCASMINFO_H
#define LLVM_LIB_TARGET_Cpl_MCTARGETDESC_CplMCASMINFO_H

#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
  class CplMCAsmInfo : public MCAsmInfoELF {
    void anchor() override;
  public:
    explicit CplMCAsmInfo(StringRef TT);
  };

} // namespace llvm

#endif

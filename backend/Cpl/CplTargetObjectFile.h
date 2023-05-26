//===-- CplTargetAsmInfo.h - Cpl asm properties ----------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_CplTARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_Cpl_CplTARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/MC/MCSectionELF.h"

namespace llvm {

  class CplTargetObjectFile : public TargetLoweringObjectFileELF {
    const MCSectionELF *SmallDataSection;
    const MCSectionELF *SmallBSSSection;
  public:
    void Initialize(MCContext &Ctx, const TargetMachine &TM) override;

    const MCSection *SelectSectionForGlobal(const GlobalValue *GV,
                                        SectionKind Kind, Mangler &Mang,
                                        const TargetMachine &TM) const override;
  };

} // namespace llvm

#endif

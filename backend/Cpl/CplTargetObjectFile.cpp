//===-- CplTargetObjectFile.cpp - Cpl asm properties ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the CplTargetAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "CplTargetObjectFile.h"
#include "CplSubtarget.h"
#include "CplTargetMachine.h"

#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/MC/MCContext.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ELF.h"

using namespace llvm;

void CplTargetObjectFile::Initialize(MCContext &Ctx,
                                         const TargetMachine &TM) {
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
  InitializeELF(TM.Options.UseInitArray);
}

const MCSection *
CplTargetObjectFile::SelectSectionForGlobal(const GlobalValue *GV,
                                                SectionKind Kind, Mangler &Mang,
                                                const TargetMachine &TM) const {
  // Otherwise, we work the same as ELF.
  return TargetLoweringObjectFileELF::SelectSectionForGlobal(GV, Kind, Mang,TM);
}

//=-- CplTargetMachine.h - Define TargetMachine for Cpl -----------*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Cpl specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_CplTARGETMACHINE_H
#define LLVM_LIB_TARGET_Cpl_CplTARGETMACHINE_H

#include "CplInstrInfo.h"
#include "CplSubtarget.h"

#include "llvm/Target/TargetMachine.h"

namespace llvm {

class Module;

class CplTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  CplSubtarget Subtarget;

public:
  CplTargetMachine(const Target &T, StringRef TT,StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       Reloc::Model RM, CodeModel::Model CM,
                       CodeGenOpt::Level OL);
  ~CplTargetMachine() override;

  const CplSubtarget *getSubtargetImpl() const override {
    return &Subtarget;
  }
  static unsigned getModuleMatchQuality(const Module &M);

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};

} // end namespace llvm

#endif

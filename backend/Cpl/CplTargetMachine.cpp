//===-- CplTargetMachine.cpp - Define TargetMachine for Cpl -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about Cpl target spec.
//
//===----------------------------------------------------------------------===//

#include "CplTargetMachine.h"
#include "Cpl.h"
#include "CplISelLowering.h"
#include "CplTargetObjectFile.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Scalar.h"

using namespace llvm;

/// CplTargetMachineModule - Note that this is used on hosts that
/// cannot link in a library unless there are references into the
/// library.  In particular, it seems that it is not possible to get
/// things to work on Win32 without this.  Though it is unused, do not
/// remove it.
extern "C" int CplTargetMachineModule;
int CplTargetMachineModule = 0;

extern "C" void LLVMInitializeCplTarget() {
  // Register the target.
  RegisterTargetMachine<CplTargetMachine> X(TheCplTarget);
}

/// CplTargetMachine ctor - Create an ILP32 architecture model.
///

CplTargetMachine::CplTargetMachine(const Target &T, StringRef TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           Reloc::Model RM, CodeModel::Model CM,
                                           CodeGenOpt::Level OL)
    : LLVMTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL),
      TLOF(make_unique<CplTargetObjectFile>()),
      Subtarget(TT, CPU, FS, *this) {
    initAsmInfo();
}

CplTargetMachine::~CplTargetMachine() {}

namespace {
/// Cpl Code Generator Pass Configuration Options.
class CplPassConfig : public TargetPassConfig {
public:
  CplPassConfig(CplTargetMachine *TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {
  }

  CplTargetMachine &getCplTargetMachine() const {
    return getTM<CplTargetMachine>();
  }

  bool addInstSelector() override;
  void addPreRegAlloc() override;
  void addPostRegAlloc() override;
  void addPreSched2() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *CplTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new CplPassConfig(this, PM);
}

bool CplPassConfig::addInstSelector() {
  CplTargetMachine &TM = getCplTargetMachine();
  addPass(createCplISelDag(TM, getOptLevel()));
  return false;
}

void CplPassConfig::addPreRegAlloc() {
}

void CplPassConfig::addPostRegAlloc() {
}

void CplPassConfig::addPreSched2() {
}

void CplPassConfig::addPreEmitPass() {
}

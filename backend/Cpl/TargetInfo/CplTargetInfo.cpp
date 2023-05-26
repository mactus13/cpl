//===-- CplTargetInfo.cpp - Cpl Target Implementation ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Cpl.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheCplTarget;

extern "C" void LLVMInitializeCplTargetInfo() {
  RegisterTarget<Triple::cpl, /*HasJIT=*/false>  X(TheCplTarget, "cpl", "Cpl");
}

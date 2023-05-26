//===-- CplSubtarget.cpp - Cpl Subtarget Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Cpl specific subclass of TargetSubtarget.
//
//===----------------------------------------------------------------------===//

#include "CplSubtarget.h"
#include "Cpl.h"
#include "CplRegisterInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "Cpl-subtarget"

#define GET_SUBTARGETINFO_CTOR
#define GET_SUBTARGETINFO_TARGET_DESC
#include "CplGenSubtargetInfo.inc"

CplSubtarget &
CplSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS) {
  // If the programmer has not specified a Cpl version.
  if (CPUString.empty())
    CPUString = "Cpl";

  ParseSubtargetFeatures(CPUString, FS);
  return *this;
}

CplSubtarget::CplSubtarget(StringRef TT, StringRef CPU, StringRef FS,
                                   const TargetMachine &TM)
    : CplGenSubtargetInfo(TT, CPU, FS), CPUString(CPU.str()),
      DL("e-m:e-p:32:32-i1:32-i64:64-a:0-n32"),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), TLInfo(TM),
      TSInfo(DL), FrameLowering() {

  // Initialize scheduling itinerary for the specified CPU.
  InstrItins = getInstrItineraryForCPU(CPUString);
}

// Pin the vtable to this file.
void CplSubtarget::anchor() {}

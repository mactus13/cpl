//===-- CplMCTargetDesc.cpp - Cpl Target Descriptions ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Cpl specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "CplMCTargetDesc.h"
#include "CplMCAsmInfo.h"
#include "MCTargetDesc/CplInstPrinter.h"
#include "MCTargetDesc/CplMCInst.h"
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "CplGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "CplGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "CplGenRegisterInfo.inc"

static MCInstrInfo *createCplMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitCplMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createCplMCRegisterInfo(StringRef TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitCplMCRegisterInfo(X, Cpl::R0);
  return X;
}

static MCStreamer *
createCplELFStreamer(MCContext &Context, MCAsmBackend &MAB,
                         raw_ostream &OS, MCCodeEmitter *CE,
                         bool RelaxAll) {
  MCELFStreamer *ES = new MCELFStreamer(Context, MAB, OS, CE);
  return ES;
}


static MCSubtargetInfo *
createCplMCSubtargetInfo(StringRef TT, StringRef CPU, StringRef FS) {
  MCSubtargetInfo *X = new MCSubtargetInfo();
  InitCplMCSubtargetInfo(X, TT, CPU, FS);
  return X;
}

static MCAsmInfo *createCplMCAsmInfo(const MCRegisterInfo &MRI,
                                         StringRef TT) {
  MCAsmInfo *MAI = new CplMCAsmInfo(TT);
  return MAI;
}

static MCStreamer *createMCStreamer(Target const &T, StringRef TT,
                                    MCContext &Context, MCAsmBackend &MAB,
                                    raw_ostream &OS, MCCodeEmitter *Emitter,
                                    MCSubtargetInfo const &STI, bool RelaxAll) {
  MCStreamer *ES = createCplELFStreamer(Context, MAB, OS, Emitter, RelaxAll);
  new MCTargetStreamer(*ES);
  return ES;
}


static MCCodeGenInfo *createCplMCCodeGenInfo(StringRef TT, Reloc::Model RM,
                                                 CodeModel::Model CM,
                                                 CodeGenOpt::Level OL) {
  MCCodeGenInfo *X = new MCCodeGenInfo();
  // For the time being, use static relocations, since there's really no
  // support for PIC yet.
  X->InitMCCodeGenInfo(Reloc::Static, CM, OL);
  return X;
}
static MCInstPrinter *createCplMCInstPrinter(const Target &T,
                                                 unsigned SyntaxVariant,
                                                 const MCAsmInfo &MAI,
                                                 const MCInstrInfo &MII,
                                                 const MCRegisterInfo &MRI,
                                                 const MCSubtargetInfo &STI) {
    return new CplInstPrinter(MAI, MII, MRI);
}

// Force static initialization.
extern "C" void LLVMInitializeCplTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfoFn X(TheCplTarget, createCplMCAsmInfo);

  // Register the MC codegen info.
  TargetRegistry::RegisterMCCodeGenInfo(TheCplTarget,
                                        createCplMCCodeGenInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(TheCplTarget,
                                      createCplMCInstrInfo);
  CplMCInst::MCII.reset (createCplMCInstrInfo());

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheCplTarget,
                                    createCplMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(TheCplTarget,
                                          createCplMCSubtargetInfo);

#ifdef TODO_CPL
  // Register the MC Code Emitter
  TargetRegistry::RegisterMCCodeEmitter(TheCplTarget,
                                        createCplMCCodeEmitter);
#endif

  // Register the MC Inst Printer
  TargetRegistry::RegisterMCInstPrinter(TheCplTarget,
                                        createCplMCInstPrinter);

#ifdef TODO_CPL
  // Register the asm backend
  TargetRegistry::RegisterMCAsmBackend(TheCplTarget,
                                       createCplAsmBackend);
#endif

  // Register the obj streamer
  TargetRegistry::RegisterMCObjectStreamer(TheCplTarget, createMCStreamer);
}

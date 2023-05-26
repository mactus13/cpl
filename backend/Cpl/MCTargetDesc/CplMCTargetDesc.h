//===-- CplMCTargetDesc.h - Cpl Target Descriptions -------------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_Cpl_MCTARGETDESC_CplMCTARGETDESC_H
#define LLVM_LIB_TARGET_Cpl_MCTARGETDESC_CplMCTARGETDESC_H

#include <cstdint>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class Target;
class StringRef;
class raw_ostream;

extern Target TheCplTarget;

MCCodeEmitter *createCplMCCodeEmitter(MCInstrInfo const &MCII,
                                          MCRegisterInfo const &MRI,
                                          MCSubtargetInfo const &MST,
                                          MCContext &MCT);

MCAsmBackend *createCplAsmBackend(Target const &T,
                                      MCRegisterInfo const &MRI, StringRef TT,
                                      StringRef CPU);

MCObjectWriter *createCplELFObjectWriter(raw_ostream &OS, uint8_t OSABI,
                                             StringRef CPU);

} // End llvm namespace

// Define symbolic names for Cpl registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "CplGenRegisterInfo.inc"

// Defines symbolic names for the Cpl instructions.
//
#define GET_INSTRINFO_ENUM
#include "CplGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "CplGenSubtargetInfo.inc"

#endif

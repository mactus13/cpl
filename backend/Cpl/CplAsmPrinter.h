//===-- CplAsmPrinter.h - Print machine code to an Cpl .s file ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Cpl Assembly printer class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_CplASMPRINTER_H
#define LLVM_LIB_TARGET_Cpl_CplASMPRINTER_H

#include "Cpl.h"
#include "CplTargetMachine.h"

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
  class CplAsmPrinter : public AsmPrinter {
    const CplSubtarget *Subtarget;

  public:
    explicit CplAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
      : AsmPrinter(TM, Streamer) {
      Subtarget = &TM.getSubtarget<CplSubtarget>();
    }

    const char *getPassName() const override {
      return "Cpl Assembly Printer";
    }

    void EmitInstruction(const MachineInstr *MI) override;
  };

} // end of llvm namespace

#endif

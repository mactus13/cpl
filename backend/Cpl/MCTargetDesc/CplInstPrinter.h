//===-- CplInstPrinter.h - Convert Cpl MCInst to assembly syntax ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an Cpl MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_INSTPRINTER_CplINSTPRINTER_H
#define LLVM_LIB_TARGET_Cpl_INSTPRINTER_CplINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"

namespace llvm {
  class CplMCInst;

  class CplInstPrinter : public MCInstPrinter {
  public:
    explicit CplInstPrinter(const MCAsmInfo &MAI,
                                const MCInstrInfo &MII,
                                const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI), MII(MII) {}

    void printInst(const MCInst *MI, raw_ostream &O, StringRef Annot) override;
    void printInst(const CplMCInst *MI, raw_ostream &O, StringRef Annot);
    virtual StringRef getOpcodeName(unsigned Opcode) const;
    void printInstruction(const MCInst *MI, raw_ostream &O);
    StringRef getRegName(unsigned RegNo) const;
    static const char *getRegisterName(unsigned RegNo);

    void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O) const;

    const MCInstrInfo &getMII() const {
      return MII;
    }

  private:
    const MCInstrInfo &MII;

  };

} // end namespace llvm

#endif

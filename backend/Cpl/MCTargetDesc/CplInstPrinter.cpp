//===- CplInstPrinter.cpp - Convert Cpl MCInst to assembly syntax ---------===//
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

#include "CplAsmPrinter.h"
#include "Cpl.h"
#include "CplInstPrinter.h"
#include "MCTargetDesc/CplMCInst.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define GET_INSTRUCTION_NAME
#include "CplGenAsmWriter.inc"

StringRef CplInstPrinter::getOpcodeName(unsigned Opcode) const {
  return MII.getName(Opcode);
}

StringRef CplInstPrinter::getRegName(unsigned RegNo) const {
  return getRegisterName(RegNo);
}

void CplInstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                   StringRef Annot) {
  printInst((const CplMCInst*)(MI), O, Annot);
}

void CplInstPrinter::printInst(const CplMCInst *MI, raw_ostream &O,
                                   StringRef Annot) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void CplInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O) const {
  const MCOperand& MO = MI->getOperand(OpNo);

  if (MO.isReg()) {
    O << getRegisterName(MO.getReg());
  } else if(MO.isExpr()) {
    O << *MO.getExpr();
  } else if(MO.isImm()) {
    O << "#" << MO.getImm();
  } else {
    llvm_unreachable("Unknown operand");
  }
}

//===-- CplAsmPrinter.cpp - Print machine instrs to Cpl assembly ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to Cpl assembly language. This printer is
// the output mechanism used by `llc'.
//
//===----------------------------------------------------------------------===//

#include "Cpl.h"
#include "CplAsmPrinter.h"
#include "CplMachineFunctionInfo.h"
#include "CplSubtarget.h"
#include "CplTargetMachine.h"
#include "MCTargetDesc/CplInstPrinter.h"
#include "MCTargetDesc/CplMCInst.h"

#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

void CplLowerToMC(const MachineInstr* MI, CplMCInst& MCI,
  CplAsmPrinter& AP);

/// printMachineInstruction -- Print out a single Cpl MI to the current
/// output stream.
///
void CplAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  CplMCInst TmpInst0;
  CplLowerToMC(MI, TmpInst0, *this);

  // MOV_I32 is a pseudo instruction that should never be emitted.
  // In order to initialize a register with a 32 bit value (Immediate or
  // Symbol), we emit the data after a jump and we load from there.
  // The code will look like this (r15 is the program counter):
  //    ldr r0, [r15, #0]               @ MOV_I32
  //    mov r15, r15
  //    .long	dump_int
  if (TmpInst0.getOpcode() == Cpl::MOV_I32) {

    const TargetRegisterInfo *TRI = MI->getParent()->getParent()->getSubtarget().getRegisterInfo();
    unsigned PC = TRI->getProgramCounter();

    CplMCInst LD;
    LD.setOpcode(Cpl::LDR);
    LD.addOperand(TmpInst0.getOperand(0));
    LD.addOperand(MCOperand::CreateReg(PC));
    LD.addOperand(MCOperand::CreateImm(0));

    CplMCInst BR;
    BR.setOpcode(Cpl::MOVR);
    BR.addOperand(MCOperand::CreateReg(PC));
    BR.addOperand(MCOperand::CreateReg(PC));

    if (isVerbose()) {
      OutStreamer.AddComment("MOV_I32");
    }

    OutStreamer.EmitInstruction(LD, getSubtargetInfo());
    OutStreamer.EmitInstruction(BR, getSubtargetInfo());
    if (TmpInst0.getOperand(1).isExpr()) {
      const MCSymbolRefExpr *Expr = dyn_cast<const MCSymbolRefExpr>(
        TmpInst0.getOperand(1).getExpr());
      OutStreamer.EmitSymbolValue(&Expr->getSymbol(), 4);
    }
    else if (TmpInst0.getOperand(1).isImm()) {
      OutStreamer.EmitIntValue(TmpInst0.getOperand(1).getImm(), 4);
    }
    else {
      llvm_unreachable("Invalid MOV_I32 operand type");
    }
    return;
  }

  if (isVerbose()) {
    if (MI->isCall()) {
      OutStreamer.AddComment("Call");
    }
    if (MI->isReturn()) {
      OutStreamer.AddComment("Return");
    }
  }
  OutStreamer.EmitInstruction(TmpInst0, getSubtargetInfo());
}

extern "C" void LLVMInitializeCplAsmPrinter() {
  RegisterAsmPrinter<CplAsmPrinter> X(TheCplTarget);
}

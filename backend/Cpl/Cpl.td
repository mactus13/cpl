//===-- Cpl.td - Describe the Cpl Target Machine ----------*- tablegen -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This is the top level entry point for the Cpl target.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

include "CplRegisterInfo.td"
include "CplOperands.td"
include "CplCallingConv.td"
include "CplInstrFormat.td"
include "CplInstrInfo.td"
include "CplPatterns.td"

def CplInstrInfo : InstrInfo;

class Proc<string Name, SchedMachineModel Model,
           list<SubtargetFeature> Features>
 : ProcessorModel<Name, Model, Features>;

def : Proc<"Cpl", NoSchedModel, []>;


def Cpl : Target {
  // Pull in Instruction Info:
  let InstructionSet = CplInstrInfo;
}

//===-- CplISelLowering.h - Cpl DAG Lowering Interface ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Cpl uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_CplISELLOWERING_H
#define LLVM_LIB_TARGET_Cpl_CplISELLOWERING_H

#include "Cpl.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/Target/TargetLowering.h"

namespace llvm {

  namespace CplISD {
    enum {
      FIRST_NUMBER = ISD::BUILTIN_OP_END,
      CALL,
      RET,
      BOA,
      MOVi32,
    };
  }

  class CplTargetLowering : public TargetLowering {
  public:
    const TargetMachine &TM;
    explicit CplTargetLowering(const TargetMachine &targetmachine);

    SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

    const char *getTargetNodeName(unsigned Opcode) const override;

    SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                      SmallVectorImpl<SDValue> &InVals) const override;

    SDValue LowerCallResult(SDValue Chain, SDValue InFlag,
                            CallingConv::ID CallConv, bool isVarArg,
                            const SmallVectorImpl<ISD::InputArg> &Ins,
                            SDLoc dl, SelectionDAG &DAG,
                            SmallVectorImpl<SDValue> &InVals,
                            const SmallVectorImpl<SDValue> &OutVals,
                            SDValue Callee) const;

    SDValue LowerReturn(SDValue Chain,
                        CallingConv::ID CallConv, bool isVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        const SmallVectorImpl<SDValue> &OutVals,
                        SDLoc dl, SelectionDAG &DAG) const override;

    SDValue LowerFormalArguments(SDValue Chain,
                                 CallingConv::ID CallConv,
                                 bool isVarArg,
                                 const SmallVectorImpl<ISD::InputArg> &Ins,
                                 SDLoc dl, SelectionDAG &DAG,
                                 SmallVectorImpl<SDValue> &InVals) const override;

    SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

    SDValue LowerShift(SDValue Op, SelectionDAG &DAG) const;

  };
} // end namespace llvm

#endif    // Cpl_ISELLOWERING_H

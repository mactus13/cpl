//===-- CplISelLowering.cpp - Cpl DAG Lowering Implementation -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the interfaces that Cpl uses to lower LLVM code
// into a selection DAG.
//
//===----------------------------------------------------------------------===//

#include "CplISelLowering.h"
#include "CplMachineFunctionInfo.h"
#include "CplSubtarget.h"
#include "CplTargetMachine.h"
#include "CplTargetObjectFile.h"

#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalAlias.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "CplGenCallingConv.inc"

#define DEBUG_TYPE "Cpl-lowering"


// LowerReturn - Lower ISD::RET. If a struct is larger than 8 bytes and is
// passed by value, the function prototype is modified to return void and
// the value is stored in memory pointed by a pointer passed by caller.
SDValue
CplTargetLowering::LowerReturn(SDValue Chain,
                                   CallingConv::ID CallConv, bool isVarArg,
                                   const SmallVectorImpl<ISD::OutputArg> &Outs,
                                   const SmallVectorImpl<SDValue> &OutVals,
                                   SDLoc dl, SelectionDAG &DAG) const {

  MachineFunction &MF = DAG.getMachineFunction();
  CplMachineFunctionInfo *FuncInfo = MF.getInfo<CplMachineFunctionInfo>();
  const CplRegisterInfo *QRI = static_cast<const CplRegisterInfo *>(
      DAG.getSubtarget().getRegisterInfo());

  // CCValAssign - represent the assignment of the return value to locations.
  SmallVector<CCValAssign, 16> RVLocs;

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, isVarArg, MF, RVLocs,
                 *DAG.getContext());

  // Analyze return values of ISD::RET
  CCInfo.AnalyzeReturn(Outs, RetCC_Cpl);

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  Chain = DAG.getCopyToReg(Chain, dl, QRI->getLinkRegister(), DAG.getRegister(FuncInfo->getRA(), MVT::i32), Flag);
  Flag = Chain.getValue(1);
  RetOps.push_back(DAG.getRegister(QRI->getLinkRegister(), MVT::i32));

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    CCValAssign &VA = RVLocs[i];

    Chain = DAG.getCopyToReg(Chain, dl, VA.getLocReg(), OutVals[i], Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain;  // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  return DAG.getNode(CplISD::RET, dl, MVT::Other, RetOps);

  return Chain;
}




/// LowerCallResult - Lower the result values of an ISD::CALL into the
/// appropriate copies out of appropriate physical registers.  This assumes that
/// Chain/InFlag are the input chain/flag to use, and that TheCall is the call
/// being lowered. Returns a SDNode with the same number of values as the
/// ISD::CALL.
SDValue
CplTargetLowering::LowerCallResult(SDValue Chain, SDValue InFlag,
                                       CallingConv::ID CallConv, bool isVarArg,
                                       const
                                       SmallVectorImpl<ISD::InputArg> &Ins,
                                       SDLoc dl, SelectionDAG &DAG,
                                       SmallVectorImpl<SDValue> &InVals,
                                       const SmallVectorImpl<SDValue> &OutVals,
                                       SDValue Callee) const {

  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;

  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeCallResult(Ins, RetCC_Cpl);

  // Copy all of the result registers out of their specified physreg.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    Chain = DAG.getCopyFromReg(Chain, dl,
                               RVLocs[i].getLocReg(),
                               RVLocs[i].getValVT(), InFlag).getValue(1);
    InFlag = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
  }

  return Chain;
}

/// LowerCall - Functions arguments are copied from virtual regs to
/// (physical regs)/(stack frame), CALLSEQ_START and CALLSEQ_END are emitted.
SDValue
CplTargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                 SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG                     = CLI.DAG;
  SDLoc &dl                             = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals     = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins   = CLI.Ins;
  SDValue Chain                         = CLI.Chain;
  SDValue Callee                        = CLI.Callee;
  bool &isTailCall                      = CLI.IsTailCall;
  CallingConv::ID CallConv              = CLI.CallConv;
  bool isVarArg                         = CLI.IsVarArg;

  isTailCall = false;

  // Analyze operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                        *DAG.getContext());

  CCInfo.AnalyzeCallOperands(Outs, CC_Cpl);

  // Get a count of how many bytes are to be pushed on the stack.
  unsigned NumBytes = CCInfo.getNextStackOffset();
  SmallVector<std::pair<unsigned, SDValue>, 16> RegsToPass;
  SmallVector<SDValue, 8> MemOpChains;

  const CplRegisterInfo *QRI = static_cast<const CplRegisterInfo *>(
      DAG.getSubtarget().getRegisterInfo());
  SDValue StackPtr =
      DAG.getCopyFromReg(Chain, dl, QRI->getStackRegister(), getPointerTy());

  // Walk the register/memloc assignments, inserting copies/loads.
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    SDValue Arg = OutVals[i];
    ISD::ArgFlagsTy Flags = Outs[i].Flags;

    // Promote the value if needed.
    switch (VA.getLocInfo()) {
      default:
        // Loc info must be one of Full, SExt, ZExt, or AExt.
        llvm_unreachable("Unknown loc info!");
      case CCValAssign::Full:
        break;
      case CCValAssign::SExt:
        Arg = DAG.getNode(ISD::SIGN_EXTEND, dl, VA.getLocVT(), Arg);
        break;
      case CCValAssign::ZExt:
        Arg = DAG.getNode(ISD::ZERO_EXTEND, dl, VA.getLocVT(), Arg);
        break;
      case CCValAssign::AExt:
        Arg = DAG.getNode(ISD::ANY_EXTEND, dl, VA.getLocVT(), Arg);
        break;
    }

    if (VA.isMemLoc()) {
      unsigned LocMemOffset = VA.getLocMemOffset();
      SDValue PtrOff = DAG.getConstant(LocMemOffset, StackPtr.getValueType());
      PtrOff = DAG.getNode(ISD::ADD, dl, MVT::i32, StackPtr, PtrOff);

      if (Flags.isByVal()) {
        // The argument is a struct passed by value. According to LLVM, "Arg"
        // is is pointer.
#ifdef TODO_CPL
        MemOpChains.push_back(CreateCopyOfByValArgument(Arg, PtrOff, Chain,
                                                        Flags, DAG, dl));
#endif
      } else {
        // The argument is not passed by value. "Arg" is a buildin type. It is
        // not a pointer.
        MemOpChains.push_back(DAG.getStore(Chain, dl, Arg, PtrOff,
                                           MachinePointerInfo(),false, false,
                                           0));
      }
      continue;
    }

    // Arguments that can be passed on register must be kept at RegsToPass
    // vector.
    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
    }
  }

  // Transform all store nodes into one single node because all store
  // nodes are independent of each other.
  if (!MemOpChains.empty()) {
    Chain = DAG.getNode(ISD::TokenFactor, dl, MVT::Other, MemOpChains);
  }

    Chain = DAG.getCALLSEQ_START(Chain, DAG.getConstant(NumBytes,
                                                        getPointerTy(), true),
                                 dl);

  // Build a sequence of copy-to-reg nodes chained together with token
  // chain and flag operands which copy the outgoing args into registers.
  // The InFlag in necessary since all emitted instructions must be
  // stuck together.
  SDValue InFlag;
  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Chain = DAG.getCopyToReg(Chain, dl, RegsToPass[i].first,
                              RegsToPass[i].second, InFlag);
    InFlag = Chain.getValue(1);
  }

  // If the callee is a GlobalAddress/ExternalSymbol node (quite common, every
  // direct call is) turn it into a TargetGlobalAddress/TargetExternalSymbol
  // node so that legalize doesn't hack it.
  if (GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee)) {
    Callee = DAG.getTargetGlobalAddress(G->getGlobal(), dl, getPointerTy());
  } else if (ExternalSymbolSDNode *S =
             dyn_cast<ExternalSymbolSDNode>(Callee)) {
    Callee = DAG.getTargetExternalSymbol(S->getSymbol(), getPointerTy());
  }

  // Returns a chain & a flag for retval copy to use.
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  // Add argument registers to the end of the list so that they are
  // known live into the call.
  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Ops.push_back(DAG.getRegister(RegsToPass[i].first,
                                  RegsToPass[i].second.getValueType()));
  }

  // Add a register mask operand representing the call - preserved registers.
  const TargetRegisterInfo *TRI = DAG.getMachineFunction().getSubtarget().getRegisterInfo();
  const uint32_t *Mask = TRI->getCallPreservedMask(CallConv);
  assert(Mask && "Missing call preserved mask for calling convention");

  Ops.push_back(CLI.DAG.getRegisterMask(Mask));

  if (InFlag.getNode()) {
    Ops.push_back(InFlag);
  }

  Chain = DAG.getNode(CplISD::CALL, dl, NodeTys, Ops);
  InFlag = Chain.getValue(1);

  // Create the CALLSEQ_END node.
  Chain = DAG.getCALLSEQ_END(Chain, DAG.getIntPtrConstant(NumBytes, true),
                             DAG.getIntPtrConstant(0, true), InFlag, dl);
  InFlag = Chain.getValue(1);

  // Handle result values, copying them out of physregs into vregs that we
  // return.
  return LowerCallResult(Chain, InFlag, CallConv, isVarArg, Ins, dl, DAG,
                         InVals, OutVals, Callee);
}

SDValue
CplTargetLowering::LowerFormalArguments(SDValue Chain,
                                            CallingConv::ID CallConv,
                                            bool isVarArg,
                                            const
                                            SmallVectorImpl<ISD::InputArg> &Ins,
                                            SDLoc dl, SelectionDAG &DAG,
                                            SmallVectorImpl<SDValue> &InVals)
const {

  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo *MFI = MF.getFrameInfo();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();
  CplMachineFunctionInfo *FuncInfo =
    MF.getInfo<CplMachineFunctionInfo>();

  unsigned VLR = RegInfo.createVirtualRegister(&Cpl::IntRegsRegClass);
  RegInfo.addLiveIn(MF.getSubtarget().getRegisterInfo()->getRARegister(), VLR);
  DAG.getCopyFromReg(Chain, dl, VLR, MVT::i32);
  FuncInfo->setRA(VLR);

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeFormalArguments(Ins, CC_Cpl);

  // For LLVM, in the case when returning a struct by value (>8byte),
  // the first argument is a pointer that points to the location on caller's
  // stack where the return value will be stored. For Cpl, the location on
  // caller's stack is passed only when the struct size is smaller than (and
  // equal to) 8 bytes. If not, no address will be passed into callee and
  // callee return the result direclty through R0/R1.

  SmallVector<SDValue, 4> MemOps;

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    ISD::ArgFlagsTy Flags = Ins[i].Flags;
    unsigned ObjSize;
    unsigned StackLocation;
    int FI;

    if (   (VA.isRegLoc() && !Flags.isByVal())
        || (VA.isRegLoc() && Flags.isByVal() && Flags.getByValSize() > 8)) {
      // Arguments passed in registers
      // 1. int, long long, ptr args that get allocated in register.
      // 2. Large struct that gets an register to put its address in.
      EVT RegVT = VA.getLocVT();
      if (RegVT == MVT::i8 || RegVT == MVT::i16 ||
          RegVT == MVT::i32 || RegVT == MVT::f32) {
        unsigned VReg =
          RegInfo.createVirtualRegister(&Cpl::IntRegsRegClass);
        RegInfo.addLiveIn(VA.getLocReg(), VReg);
        InVals.push_back(DAG.getCopyFromReg(Chain, dl, VReg, RegVT));
      } else {
        assert (0);
      }
    } else if (VA.isRegLoc() && Flags.isByVal() && Flags.getByValSize() <= 8) {
      assert (0 && "ByValSize must be bigger than 8 bytes");
    } else {
      // Sanity check.
      assert(VA.isMemLoc());

      if (Flags.isByVal()) {
        // If it's a byval parameter, then we need to compute the
        // "real" size, not the size of the pointer.
        ObjSize = Flags.getByValSize();
      } else {
        ObjSize = VA.getLocVT().getStoreSizeInBits() >> 3;
      }

      StackLocation = VA.getLocMemOffset();
      // Create the frame index object for this incoming parameter...
      FI = MFI->CreateFixedObject(ObjSize, StackLocation, true);

      // Create the SelectionDAG nodes cordl, responding to a load
      // from this parameter.
      SDValue FIN = DAG.getFrameIndex(FI, MVT::i32);

      if (Flags.isByVal()) {
        // If it's a pass-by-value aggregate, then do not dereference the stack
        // location. Instead, we should generate a reference to the stack
        // location.
        InVals.push_back(FIN);
      } else {
        InVals.push_back(DAG.getLoad(VA.getLocVT(), dl, Chain, FIN,
                                     MachinePointerInfo(), false, false,
                                     false, 0));
      }
    }
  }

  if (!MemOps.empty())
    Chain = DAG.getNode(ISD::TokenFactor, dl, MVT::Other, MemOps);

  return Chain;
}

//===----------------------------------------------------------------------===//
// TargetLowering Implementation
//===----------------------------------------------------------------------===//

CplTargetLowering::CplTargetLowering(const TargetMachine &targetmachine)
    : TargetLowering(targetmachine),
      TM(targetmachine) {

  setBooleanContents(ZeroOrNegativeOneBooleanContent);

  // Set up the register classes.
  addRegisterClass(MVT::i32, &Cpl::IntRegsRegClass);
  computeRegisterProperties();

  setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
  setOperationAction(ISD::BR_CC, MVT::i32, Expand);
  setOperationAction(ISD::SELECT_CC, MVT::i32, Expand);

  setOperationAction(ISD::SRA, MVT::i32, Custom);
  setOperationAction(ISD::SHL, MVT::i32, Custom);
  setOperationAction(ISD::SRL, MVT::i32, Custom);

  setOperationAction(ISD::SDIV, MVT::i32, Expand);
  setOperationAction(ISD::UDIV, MVT::i32, Expand);
  setOperationAction(ISD::SREM, MVT::i32, Expand);
  setOperationAction(ISD::UREM, MVT::i32, Expand);
  setOperationAction(ISD::SDIVREM, MVT::i32, Expand);
  setOperationAction(ISD::UDIVREM, MVT::i32, Expand);
}

const char*
CplTargetLowering::getTargetNodeName(unsigned Opcode) const {
#define OPCODENAME(T) case CplISD::T: return "CplISD::" # T
  switch (Opcode) {
    OPCODENAME(CALL);
    OPCODENAME(RET);
    OPCODENAME(BOA);
    OPCODENAME(MOVi32);
    default: return nullptr;
  }
#undef OPCODENAME
}

SDValue CplTargetLowering::LowerGlobalAddress(SDValue Op,
  SelectionDAG &DAG) const {

  SDLoc DL(Op);
  EVT Ty = Op.getValueType();
  GlobalAddressSDNode *N = cast<GlobalAddressSDNode>(Op);
  SDValue GA = DAG.getTargetGlobalAddress(N->getGlobal(), SDLoc(N), Ty, 0);
  SDValue Of = DAG.getConstant(N->getOffset(), MVT::i32);
  return DAG.getNode(CplISD::BOA, DL, Ty, Of, GA);
}

SDValue CplTargetLowering::LowerShift(SDValue Op,
  SelectionDAG &DAG) const {
  SDLoc DL(Op);
  EVT Ty = Op.getValueType();
  SDValue Op0 = Op.getOperand(0);
  SDValue Op1 = Op.getOperand(1);
  SDValue Val;

  if (!isa<ConstantSDNode>(Op1))
  {
	llvm_unreachable("Unsupported shift offset");
  }

  ConstantSDNode *CN = cast<ConstantSDNode>(Op1);
  uint64_t Offset = CN->getSExtValue();
  Op1 = DAG.getConstant(pow(2, Offset), MVT::i32);
  Op1 = DAG.getNode(CplISD::MOVi32, DL, Ty, Op1);

  SDValue Ops[] = { Op0, Op1 };
  if (Op.getOpcode() == ISD::SRA)
  {
    uint32_t Mask = ((1 << (uint32_t)Offset) - 1) << (32 - (uint32_t)Offset);
    SDValue Cmp = DAG.getSetCC(DL, Ty, Op0, DAG.getConstant(0, MVT::i32), ISD::SETLT);
    Ops[0] = DAG.getNode(ISD::UDIV, DL, Ty, Ops);
    SDValue AndOps[] = {Cmp, DAG.getConstant(Mask, MVT::i32)};
    Ops[1] = DAG.getNode(ISD::AND, DL, Ty, AndOps);
    Op = DAG.getNode(ISD::OR, DL, Ty, Ops);
  }
  else if (Op.getOpcode() == ISD::SRL)
  {
    Op = DAG.getNode(ISD::UDIV, DL, Ty, Ops);
  }
  else if (Op.getOpcode() == ISD::SHL)
  {
    Op = DAG.getNode(ISD::MUL, DL, Ty, Ops);
  }

  return Op;
}

SDValue
CplTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::GlobalAddress:      return LowerGlobalAddress(Op, DAG);
  case ISD::SRA:
  case ISD::SRL:
  case ISD::SHL:
    return LowerShift(Op, DAG);
  default: llvm_unreachable("Should not custom lower this!");
  }
}

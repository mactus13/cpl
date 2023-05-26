//==- CplInstrInfo.td - Target Description for Cpl ---------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the Cpl instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

def SDT_SPCallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i32> ]>;

def SDT_SPCallSeqEnd : SDCallSeqEnd<[ SDTCisVT<0, i32>,
                                        SDTCisVT<1, i32> ]>;

def SDT_Call      : SDTypeProfile<0, 1, [SDTCisVT<0, iPTR>]>;

def callseq_end : SDNode<"ISD::CALLSEQ_END",   SDT_SPCallSeqEnd,
                  [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_SPCallSeqStart,
                  [SDNPHasChain, SDNPOutGlue]>;

// Return
def CplRet : SDNode<"CplISD::RET", SDTBrind,
                  [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

// Call
def CplCall : SDNode<"CplISD::CALL", SDT_Call,
                  [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;

// Boa
def CplBoa : SDNode<"CplISD::BOA", SDTIntBinOp>;

// Pseudo instructions.
def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt),
                      "Should never be emitted",
                      [(callseq_start timm:$amt)]>;

def ADJCALLSTACKUP : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                    "Should never be emitted",
                    [(callseq_end timm:$amt1, timm:$amt2)]>;

def frame_addr : ComplexPattern<iPTR, 2, "SelectAddr", [frameindex], [SDNPWantParent]>;

def FRAME : Pseudo<(outs IntRegs:$rc),
             (ins mem:$ra), "Should never be emitted",
             [(set IntRegs:$rc, frame_addr:$ra)]>
{
}

let isReturn=1, isTerminator=1, isBarrier=1, isCodeGenOnly=1 in {
  def RET : Pseudo<(outs), (ins IntRegs:$RA),
                   "Should never be emitted",
                   [(CplRet IntRegs:$RA)]>;
  def ORR_RET : InstCplALUdss<"orr", 0b1100>;
}

let isCall=1, isCodeGenOnly=1, Uses=[LR.Val], Defs=[LR.Val] in {
  def CALLR : Pseudo<(outs), (ins IntRegs:$RA),
             "Should never be emitted", []>;

  def ORR_CALL : InstCplALUdss<"orr", 0b1100>;
}

def MOV_I32 : Pseudo<(outs IntRegs:$RD), (ins i32imm:$imm),
                   "Should never be emitted",
                   [(set IntRegs:$RD, s32ImmPred:$imm)]> {
  let isMoveImm = 1;
}

class COMP : Pseudo<(outs IntRegs:$rc),
             (ins IntRegs:$ra, IntRegs:$rb), "Should never be emitted">
{
  let Defs = [CPSR];
  let isCompare = 1;
}

class Is_Branch {
  bit isBranch = 1;
  bit isTerminator = 1;
  bit isBarrier = 1;
}

class Is_Conditioned_Branch {
  bit isBranch = 1;
  bit isTerminator = 1;
  bit isPredicated = 1; 
}

class Is_Call {
  bit isCall = 1;
}

class Is_Compare {
  bit isCompare = 1;
}

class Is_Conditioned {
  bit isPredicated = 1;
  list<Register> Uses = [CPSR];
}

// TODO 1 : Define Cpl instructions by instantiating the classes defined in
// Am definit toate instructiunile care compun setul de instructiuni al arhitecturii CPL,
//  marcand corespunzator instructiunile precizate in enunt.
def ADDR : InstCplALUdss <"add", 0>;
def ADDI : InstCplALUdsi <"add", 0>;

def SUBR : InstCplALUdss <"sub", 0>;
def SUBI : InstCplALUdsi <"sub", 0>;

def ANDR : InstCplALUdss <"and", 0>;
def ANDI : InstCplALUdsi <"and", 0>;

def ORRR : InstCplALUdss <"orr", 0>;
def ORRI : InstCplALUdsi <"orr", 0>;

def EORR : InstCplALUdss <"eor", 0>;
def EORI : InstCplALUdsi <"eor", 0>;

def MOVR : InstCplALUds <"mov", 0>;
def MOVI : InstCplALUdi <"mov", 0>;

def MVNR : InstCplALUds <"mvn", 0>;
def MVNI : InstCplALUdi <"mvn", 0>;

def CMPR : InstCplCmpss <"cmp", 0>, Is_Compare;
def CMPI : InstCplCmpsi <"cmp", 0>, Is_Compare;

def MOVEQI : InstCplALUdi <"moveq", 0>, Is_Conditioned;
def MOVNEI : InstCplALUdi <"movne", 0>, Is_Conditioned;
def MOVGEI : InstCplALUdi <"movge", 0>, Is_Conditioned;
def MOVGTI : InstCplALUdi <"movgt", 0>, Is_Conditioned;
def MOVLEI : InstCplALUdi <"movle", 0>, Is_Conditioned;
def MOVLTI : InstCplALUdi <"movlt", 0>, Is_Conditioned;

def MUL : InstCplMULdss <"mul">;

def BI : InstCplBUncond <"b", 0>, Is_Branch;
def BEQ : InstCplBCond <"beq", 0>, Is_Conditioned_Branch;
def BNE : InstCplBCond <"bne", 0>, Is_Conditioned_Branch;
def BGE : InstCplBCond <"bge", 0>, Is_Conditioned_Branch;
def BGT : InstCplBCond <"bgt", 0>, Is_Conditioned_Branch;
def BLE : InstCplBCond <"ble", 0>, Is_Conditioned_Branch;
def BLT : InstCplBCond <"blt", 0>, Is_Conditioned_Branch;

def BLI : InstCplBL <"bl">, Is_Call;

def LDR : InstCplMEMld <"ldr", 0>;
def LDRB : InstCplMEMld <"ldrb", 0>;
def STR : InstCplMEMst <"str", 0>;
def STRB : InstCplMEMst <"strb", 0>;


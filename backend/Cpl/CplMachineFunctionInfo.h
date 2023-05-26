//=- CplMachineFunctionInfo.h - Cpl machine function info ---------*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_CplMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_Cpl_CplMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/IR/ValueMap.h"
#include <map>

namespace llvm {

/// Cpl target-specific information for each MachineFunction.
class CplMachineFunctionInfo : public MachineFunctionInfo {
  // SRetReturnReg - Some subtargets require that sret lowering includes
  // returning the value of the returned struct in a register. This field
  // holds the virtual register into which the sret argument is passed.
  virtual void anchor();

  unsigned RA;

public:
  CplMachineFunctionInfo() {}

  CplMachineFunctionInfo(MachineFunction &MF) {}

  void setRA(unsigned R) {
    RA = R;
  }

  unsigned getRA() const {
    return RA;
  }

};

//@1 {
/// CplFunctionInfo - This class is derived from MachineFunction private
/// Cpl target-specific information for each MachineFunction.
class CplFunctionInfo : public MachineFunctionInfo {
public:
  CplFunctionInfo(MachineFunction& MF)
    : MF(MF), VarArgsFrameIndex(0), MaxCallFrameSize(0) {
  }

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

private:
  MachineFunction &MF;

  int VarArgsFrameIndex;

  unsigned MaxCallFrameSize;
};
//@1 }

} // End llvm namespace

#endif

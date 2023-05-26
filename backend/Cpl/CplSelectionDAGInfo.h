//===-- CplSelectionDAGInfo.h - Cpl SelectionDAG Info -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the Cpl subclass for TargetSelectionDAGInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_CplSELECTIONDAGINFO_H
#define LLVM_LIB_TARGET_Cpl_CplSELECTIONDAGINFO_H

#include "llvm/Target/TargetSelectionDAGInfo.h"

namespace llvm {

class CplSelectionDAGInfo : public TargetSelectionDAGInfo {
public:
  explicit CplSelectionDAGInfo(const DataLayout &DL);
  ~CplSelectionDAGInfo();
};

}

#endif

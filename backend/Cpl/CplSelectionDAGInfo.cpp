//===-- CplSelectionDAGInfo.cpp - Cpl SelectionDAG Info -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the CplSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#include "CplTargetMachine.h"
using namespace llvm;

#define DEBUG_TYPE "Cpl-selectiondag-info"

CplSelectionDAGInfo::CplSelectionDAGInfo(const DataLayout &DL)
    : TargetSelectionDAGInfo(&DL) {}

CplSelectionDAGInfo::~CplSelectionDAGInfo() {
}

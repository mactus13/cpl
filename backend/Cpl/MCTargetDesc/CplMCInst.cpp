//===- CplMCInst.cpp - Cpl sub-class of MCInst ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class extends MCInst to allow some Cpl VLIW annotations.
//
//===----------------------------------------------------------------------===//

#include "CplInstrInfo.h"
#include "MCTargetDesc/CplBaseInfo.h"
#include "MCTargetDesc/CplMCInst.h"
#include "MCTargetDesc/CplMCTargetDesc.h"

using namespace llvm;

std::unique_ptr<MCInstrInfo const> CplMCInst::MCII;

CplMCInst::CplMCInst() : MCInst() {}
CplMCInst::CplMCInst(MCInstrDesc const &mcid) : MCInst() {}

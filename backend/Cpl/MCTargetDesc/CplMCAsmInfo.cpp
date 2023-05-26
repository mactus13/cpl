//===-- CplMCAsmInfo.cpp - Cpl asm properties -----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the CplMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "CplMCAsmInfo.h"

using namespace llvm;

// Pin the vtable to this file.
void CplMCAsmInfo::anchor() {}

CplMCAsmInfo::CplMCAsmInfo(StringRef TT) {
  CommentString = "@";

  SupportsDebugInformation = false;
}

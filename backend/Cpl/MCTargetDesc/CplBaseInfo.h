//===-- CplBaseInfo.h - Top level definitions for Cpl ----------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the Cpl target useful for the compiler back-end and the MC libraries.
// As such, it deliberately does not include references to LLVM core
// code gen types, passes, etc..
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Cpl_MCTARGETDESC_CplBASEINFO_H
#define LLVM_LIB_TARGET_Cpl_MCTARGETDESC_CplBASEINFO_H

#include "CplMCTargetDesc.h"
#include "llvm/Support/ErrorHandling.h"
#include <stdint.h>

namespace llvm {

/// CplII - This namespace holds all of the target specific flags that
/// instruction info tracks.
///
namespace CplII {
  // *** The code below must match CplInstrFormat*.td *** //

  // MCInstrDesc TSFlags
  // *** Must match CplInstrFormat*.td ***
  enum {
    Pos = 0,
    Mask = 0x1,
  };

  // *** The code above must match CplInstrFormat*.td *** //

} // End namespace CplII.

} // End namespace llvm.

#endif

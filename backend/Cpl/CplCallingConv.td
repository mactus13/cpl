//===- CplCallingConv.td - Calling Conventions Cpl -*--------- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the Cpl architectures.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Return Value Calling Conventions
//===----------------------------------------------------------------------===//

// Cpl C return-value convention.
def RetCC_Cpl : CallingConv<[
  CCIfType<[i32, f32], CCAssignToReg<[R0]>>
]>;

// Cpl C Calling convention.
def CC_Cpl : CallingConv<[
  // TODO 2: Add register calling convention.
  // Am adaugat registrele conform calling convention-ului din enunt.
  CCIfType<[i32, f32], CCAssignToReg<[R0, R1, R2, R3]>>,

  // Alternatively, they are assigned to the stack in 4-byte aligned units.
  CCAssignToStack<4, 4>
]>;

// TODO 2: Add callee saved registers to the list.
def CSR_Cpl : CalleeSavedRegs<(add R4, R5, R6, R7, R8, R9, R10, R11)>;

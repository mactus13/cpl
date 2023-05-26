//===-- CplRegisterInfo.td - Cpl Register defs -------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the Cpl register file.
//===----------------------------------------------------------------------===//

let Namespace = "Cpl" in {

  class CplReg<bits<5> num, string n, list<string> alt = [],
                   list<Register> alias = []> : Register<n> {
    field bits<5> Num;
    let Aliases = alias;
    let HWEncoding{4-0} = num;
  }

  // Ri - 32-bit integer registers.
  class Ri<bits<5> num, string n, list<string> alt = []> : CplReg<num, n, alt> {
    let Num = num;
  }
  
  def CPSR : CplReg<0, "cpsr">;

  // Integer registers.
  foreach i = 0-15 in {
    def R#i  : Ri<i, "r"#i>,  DwarfRegNum<[i]>;
  }
}

def IntRegs : RegisterClass<"Cpl", [i32], 32,
                            (add (sequence "R%u", 0, 15))> {
}

def CPSRReg : RegisterClass<"Cpl", [i32], 32, (add CPSR)> {
}

class ReservedReg<CplReg R> {
  CplReg Val = R;
}

def SP : ReservedReg<R13>;
def LR : ReservedReg<R14>;
def PC : ReservedReg<R15>;
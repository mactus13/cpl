//==- CplInstrFormat.td - Target Description for Cpl -------*- tablegen -*-===//
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
class InstCpl<dag outs, dag ins, string asmstr>
  : Instruction {
  let Namespace = "Cpl";

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Size = 4;

  bits<32> Inst = 0;
  bits<32> SoftFail;
}

let isCodeGenOnly = 1, isPseudo = 1 in
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern = [],
             string cstr = "">
  : InstCpl<outs, ins, asmstr> {
  let Pattern = pattern;
}

class InstCplALU<dag outs, dag ins, string asmstr, bits<4> opcode>
  : InstCpl<outs, ins, asmstr> {
  let Inst{31-28} = 0b1110; // No predicates.
  let Inst{27-26} = 0;
  let Inst{24-21} = opcode;
  let Inst{20} = 0b0; // Do not write flags.
}

class InstCplMUL<dag outs, dag ins, string asmstr>
  : InstCpl<outs, ins, asmstr> {
  let Inst{31-28} = 0b1110 ; // No predicates.
  let Inst{27-22} = 0;
  let Inst{21} = 0b0; // Don't accumulate.
  let Inst{20} = 0b0; // Do not write flags.
  let Inst{15-12} = 0;
  let Inst{7-4} = 0b1001;
}

class InstCplBRANCH<dag outs, dag ins, string asmstr, bits<4> pred, bits<1> link>
  : InstCpl<outs, ins, asmstr> {
  let Inst{31-28} = pred;
  let Inst{27-25} = 0b101;
  let Inst{24} = link;
}

class InstCplMEM<dag outs, dag ins, string asmstr, bits<1> bw, bits<1> ls>
  : InstCpl<outs, ins, asmstr> {
  let Inst{31-28} = 0b1110 ; // No predicates.
  let Inst{27-26} = 0b01;
  let Inst{25} = 0b0; // Immediate offset.
  let Inst{24} = 0b1; // Pre-add immediate value.
  let Inst{23} = 0b1; // Add offset.
  let Inst{22} = bw;
  let Inst{21} = 0b0; // No write-back for the address.
  let Inst{20} = ls; // Load/Store.
}

class InstCplMEMld<string asmstr, bits<1> bw>
  : InstCplMEM<(outs IntRegs:$rd), (ins IntRegs:$rs, i32imm:$imm),
    asmstr # " $rd, [$rs, $imm]", bw, 0x1> {
  bit mayLoad = 1;
  bits<4> rd;
  bits<4> rs;
  bits<12> imm;
  let Inst{19-16} = rs;
  let Inst{15-12} = rd;
  let Inst{11-0} = imm;
}

class InstCplMEMst<string asmstr, bits<1> bw>
  : InstCplMEM<(outs), (ins IntRegs:$rd, IntRegs:$rs, i32imm:$imm),
    asmstr # " $rd, [$rs, $imm]", bw, 0x0> {
  bit mayStore = 1;
  bits<4> rd;
  bits<4> rs;
  bits<12> imm;
  let Inst{19-16} = rs;
  let Inst{15-12} = rd;
  let Inst{11-0} = imm;
}

class InstCplALUdss<string asmstr, bits<4> opcode>
  : InstCplALU<(outs IntRegs:$rd), (ins IntRegs:$rs1, IntRegs:$rs2),
    asmstr # " $rd, $rs1, $rs2", opcode> {
  bits<4> rd;
  bits<4> rs1;
  bits<4> rs2;
  let Inst{25} = 0b0;
  let Inst{19-16} = rs1;
  let Inst{15-12} = rd;
  let Inst{11-4} = 0;
  let Inst{3-0} = rs2;
}

class InstCplMULdss<string asmstr>
  : InstCplMUL<(outs IntRegs:$rd), (ins IntRegs:$rs1, IntRegs:$rs2),
    asmstr # " $rd, $rs1, $rs2"> {
  let Constraints = "$rd = $rs2";
  bits<4> rd;
  bits<4> rs1;
  bits<4> rs2;
  let Inst{25} = 0b0;
  let Inst{19-16} = rd;
  let Inst{11-8} = rs2;
  let Inst{3-0} = rs1;
}

class InstCplALUdsi<string asmstr, bits<4> opcode>
  : InstCplALU<(outs IntRegs:$rd), (ins IntRegs:$rs, i32imm:$imm),
          asmstr # " $rd, $rs, $imm", opcode> {
  bits<4> rd;
  bits<4> rs;
  bits<8> imm;
  let Inst{25} = 0b1;
  let Inst{19-16} = rs;
  let Inst{15-12} = rd;
  let Inst{11-8} = 0;
  let Inst{7-0} = imm;
}

class InstCplALUds<string asmstr, bits<4> opcode>
  : InstCplALU<(outs IntRegs:$rd), (ins IntRegs:$rs),
          asmstr # " $rd, $rs", opcode> {
  bits<4> rd;
  bits<4> rs;
  let Inst{25} = 0b0;
  let Inst{19-16} = 0;
  let Inst{15-12} = rd;
  let Inst{11-4} = 0;
  let Inst{3-0} = rs;
}

class InstCplCmpss<string asmstr, bits<4> opcode>
  : InstCplALU<(outs CPSRReg:$regc), (ins IntRegs:$rs1, IntRegs:$rs2),
          asmstr # " $rs1, $rs2", opcode> {
  bits<4> rs1;
  bits<4> rs2;
  let Inst{25} = 0b0;
  let Inst{19-16} = 0;
  let Inst{15-12} = rs1;
  let Inst{11-4} = 0;
  let Inst{3-0} = rs2;
}

class InstCplALUdi<string asmstr, bits<4> opcode>
 : InstCplALU<(outs IntRegs:$rd), (ins i32imm:$imm),
          asmstr # " $rd, $imm", opcode> {
  bits<4> rd;
  bits<8> imm;
  let Inst{25} = 0b1;
  let Inst{19-16} = 0;
  let Inst{15-12} = rd;
  let Inst{11-8} = 0;
  let Inst{7-0} = imm;
}

class InstMovCond<string qualifier, bits<4> cond>
 : InstCplALU<(outs IntRegs:$rd), (ins i32imm:$imm),
          "mov" # qualifier # " $rd, $imm", 0b1101> {
  bits<4> rd;
  bits<8> imm;
  let Inst{31-28} = cond;
  let Inst{25} = 0b1;
  let Inst{19-16} = 0;
  let Inst{15-12} = rd;
  let Inst{11-8} = 0;
  let Inst{7-0} = imm;
}

class InstCplCmpsi<string asmstr, bits<4> opcode>
 : InstCplALU<(outs CPSRReg:$regc), (ins IntRegs:$rs, i32imm:$imm),
          asmstr # " $rs, $imm", opcode> {
  bits<4> rs;
  bits<8> imm;
  let Inst{25} = 0b1;
  let Inst{19-16} = 0;
  let Inst{15-12} = rs;
  let Inst{11-8} = 0;
  let Inst{7-0} = imm;
}

class InstCplBUncond<string asmstr, bits<4> pred>
  : InstCplBRANCH<(outs), (ins jmptarget:$imm), asmstr # " $imm", pred, 0b1> {
  bits<24> imm;
  let Inst{23-0} = imm;
}

class InstCplBCond<string asmstr, bits<4> pred>
  : InstCplBRANCH<(outs), (ins CPSRReg:$regc, jmptarget:$imm), asmstr # " $imm", pred, 0b1> {
  bits<24> imm;
  let Inst{23-0} = imm;
}

class InstCplBL<string asmstr>
  : InstCplBRANCH<(outs), (ins i32imm:$imm), asmstr # " $imm", 0, 0b0> {
  bits<24> imm;
  let Inst{23-0} = imm;
}


multiclass BrCond<PatFrag cond, InstCpl inst> {
  def : Pat<(brcond (i32 (cond IntRegs:$r1, IntRegs:$r2)), bb:$dst),
            (inst (CMPR IntRegs:$r1, IntRegs:$r2), bb:$dst)>;
}

def : Pat<(br bb:$addr), (BI bb:$addr)>;

// TODO 1: Add other conditional branch patterns.
defm : BrCond<seteq, BEQ>;
defm : BrCond<setne, BNE>;
defm : BrCond<setge, BGE>;
defm : BrCond<setgt, BGT>;
defm : BrCond<setle, BLE>;
defm : BrCond<setlt, BLT>;

// TODO 1: Add pattern for addition, subtraction, and, or, exclusive or, multiplication.
// Am adaugat toate pattern-urile pentru operatiile din setul de instructiuni.
def : Pat<(add IntRegs:$ra, IntRegs:$rb), (ADDR IntRegs:$ra, IntRegs:$rb)>;
def : Pat<(add IntRegs:$ra, IntRegs:$rb), (ADDI IntRegs:$ra, IntRegs:$rb)>;

def : Pat<(sub IntRegs:$ra, IntRegs:$rb), (SUBR IntRegs:$ra, IntRegs:$rb)>;
def : Pat<(sub IntRegs:$ra, IntRegs:$rb), (SUBI IntRegs:$ra, IntRegs:$rb)>;

def : Pat<(and IntRegs:$ra, IntRegs:$rb), (ANDR IntRegs:$ra, IntRegs:$rb)>;
def : Pat<(and IntRegs:$ra, IntRegs:$rb), (ANDI IntRegs:$ra, IntRegs:$rb)>;

def : Pat<(or IntRegs:$ra, IntRegs:$rb), (ORRR IntRegs:$ra, IntRegs:$rb)>;
def : Pat<(or IntRegs:$ra, IntRegs:$rb), (ORRI IntRegs:$ra, IntRegs:$rb)>;

def : Pat<(xor IntRegs:$ra, IntRegs:$rb), (EORR IntRegs:$ra, IntRegs:$rb)>;
def : Pat<(xor IntRegs:$ra, IntRegs:$rb), (EORI IntRegs:$ra, IntRegs:$rb)>;

def : Pat<(mul IntRegs:$ra, IntRegs:$rb), (MUL IntRegs:$ra, IntRegs:$rb)>;

// TODO 3: Add pattern for select.

// Call paterns
def : Pat<(CplCall tglobaladdr:$dst), (BLI tglobaladdr:$dst)>;

def : Pat<(CplCall texternalsym:$dst), (BLI texternalsym:$dst)>;

def : Pat<(CplCall IntRegs:$ra), (CALLR IntRegs:$ra)>;

// Base offset addressing patterns.
def : Pat<(CplBoa (i32 0), tglobaladdr:$a), (MOV_I32 tglobaladdr:$a)>;

def : Pat<(CplBoa s8ImmPred:$c, tglobaladdr:$a), (ADDI (MOV_I32 tglobaladdr:$a), s8ImmPred:$c)>;

// Store pattern.
def : Pat<(store IntRegs:$rv, IntRegs:$ra), (STR IntRegs:$rv, IntRegs:$ra, 0)>;

// TODO 1: Add pattern for load.
def : Pat<(load IntRegs:$rv), (LDR IntRegs:$rv, 0)>;

// Byte-wise load patterns.
def : Pat<(extloadi8 IntRegs:$ra),  (LDRB IntRegs:$ra, 0)>;

def : Pat<(zextloadi8 IntRegs:$ra), (LDRB IntRegs:$ra, 0)>;

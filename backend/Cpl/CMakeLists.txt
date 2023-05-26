set(LLVM_TARGET_DEFINITIONS Cpl.td)

tablegen(LLVM CplGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM CplGenCallingConv.inc -gen-callingconv)
tablegen(LLVM CplGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM CplGenDFAPacketizer.inc -gen-dfa-packetizer)
tablegen(LLVM CplGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM CplGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM CplGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM CplGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM CplGenSubtargetInfo.inc -gen-subtarget)
add_public_tablegen_target(CplCommonTableGen)

add_llvm_target(CplCodeGen
  CplRegisterInfo.cpp
  CplInstrInfo.cpp
  CplAsmPrinter.cpp
  CplISelLowering.cpp
  CplFrameLowering.cpp
  CplMCInstLower.cpp
  CplISelDAGToDAG.cpp
  CplTargetObjectFile.cpp
  CplMachineFunctionInfo.cpp
  CplSelectionDAGInfo.cpp
  CplTargetMachine.cpp
  CplSubtarget.cpp
)

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)

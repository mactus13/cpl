##===- lib/Target/Cpl/Makefile -----------------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##
LEVEL = ../../..
LIBRARYNAME = LLVMCplCodeGen
TARGET = Cpl

# Make sure that tblgen is run, first thing.
BUILT_SOURCES = CplGenRegisterInfo.inc \
                CplGenInstrInfo.inc  \
                CplGenAsmWriter.inc \
                CplGenDAGISel.inc CplGenSubtargetInfo.inc \
                CplGenCallingConv.inc \
                CplGenDFAPacketizer.inc \
                CplGenMCCodeEmitter.inc \
                CplGenDisassemblerTables.inc

DIRS = TargetInfo MCTargetDesc

include $(LEVEL)/Makefile.common

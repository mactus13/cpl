add_llvm_library(LLVMCplDesc
  CplMCTargetDesc.cpp
  CplMCAsmInfo.cpp
  CplMCInst.cpp
  CplInstPrinter.cpp
)

add_dependencies(LLVMCplDesc CplCommonTableGen)

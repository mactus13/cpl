project(lcpl-semant CXX)

cmake_minimum_required(VERSION 2.8.12)


include(CheckCXXCompilerFlag)
set(CXX11_FLAG "-std=c++11" CACHE STRING "Compiler flag to enable C++ 11")
CHECK_CXX_COMPILER_FLAG(${CXX11_FLAG} HAVE_CXX11)
SET( CMAKE_CXX_COMPILER g++-5)
if (HAVE_CXX11)
  add_definitions(${CXX11_FLAG})
else()
  message(FATAL_ERROR
    "The compiler doesn't seem to have C++ 11 support (try setting the C++11 flag manually?)")
endif()

set(INCLUDE_DIR include)
include_directories(${INCLUDE_DIR})

set(SEMANT_FILES
  # Add the header files too, so they can be seen in IDE projects
  ${INCLUDE_DIR}/ASTNodes.h
  ${INCLUDE_DIR}/ASTSerialization.h
  ${INCLUDE_DIR}/ASTVisitor.h
  ${INCLUDE_DIR}/Attribute.h
  ${INCLUDE_DIR}/Assignment.h
  ${INCLUDE_DIR}/BinaryOperator.h
  ${INCLUDE_DIR}/Block.h
  ${INCLUDE_DIR}/Cast.h
  ${INCLUDE_DIR}/Class.h
  ${INCLUDE_DIR}/Dispatch.h
  ${INCLUDE_DIR}/Expression.h
  ${INCLUDE_DIR}/Feature.h
  ${INCLUDE_DIR}/FormalParam.h
  ${INCLUDE_DIR}/IfStatement.h
  ${INCLUDE_DIR}/IntConstant.h
  ${INCLUDE_DIR}/LocalDefinition.h
  ${INCLUDE_DIR}/Method.h
  ${INCLUDE_DIR}/NewObject.h
  ${INCLUDE_DIR}/Program.h
  ${INCLUDE_DIR}/StaticDispatch.h
  ${INCLUDE_DIR}/StringConstant.h
  ${INCLUDE_DIR}/Substring.h
  ${INCLUDE_DIR}/Symbol.h
  ${INCLUDE_DIR}/TreeNode.h
  ${INCLUDE_DIR}/UnaryOperator.h
  ${INCLUDE_DIR}/NullConstant.h
  ${INCLUDE_DIR}/WhileStatement.h
  ${INCLUDE_DIR}/SemanticException.h
  ${INCLUDE_DIR}/SemanticAnalysis.h
  ${INCLUDE_DIR}/Type.h
  ${INCLUDE_DIR}/SymbolTable.h
  ${INCLUDE_DIR}/TypeTable.h
  ${INCLUDE_DIR}/StringConstants.h

  src/ASTSerialization.cpp
  src/ASTVisitor.cpp
  src/SemanticAnalysis.cpp
  src/SemanticException.cpp
  src/SymbolTable.cpp
  src/TypeTable.cpp
  )

set(LCPL_SEMANT lcpl-semant)
add_library(${LCPL_SEMANT} ${SEMANT_FILES})

add_subdirectory(tests)

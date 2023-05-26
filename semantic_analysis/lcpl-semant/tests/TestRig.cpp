#include <iostream>

#include "ASTSerialization.h"
#include "SemanticAnalysis.h"
#include "TestRig.h"

Test::~Test() {}

bool Test::run(const std::string &pathToAST) {
  lcpl::ASTDeserializer deserializer(pathToAST);

  auto program = deserializer.getRootNode();
  return test(program.get());
}

void Test::createAndRunSema(lcpl::Program *program) {
  lcpl::SemanticAnalysis sema(program);
  sema.runAnalysis();
}

bool Test::pass() {
  std::cerr << "PASS" << std::endl;
  return true;
}

bool Test::fail(const std::string &message) {
  std::cerr << "FAIL: " << message << std::endl;
  return false;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << argv[0] << " <inputFile>" << std::endl;
    return -1;
  }

  auto test = getTest();
  auto passed = test->run(argv[1]);

  if (passed) {
    return 0;
  }

  return 0; // Also return 0 on FAIL in order to let make run-all continue
}

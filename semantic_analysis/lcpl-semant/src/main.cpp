
#include <iostream>

#include <ASTSerialization.h>
#include <SemanticAnalysis.h>
#include <SemanticException.h>

void printUsage() {
  std::cout << "./lcpl-semant <inputFile> [--run]" << std::endl;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    printUsage();
    return 0;
  }

  lcpl::ASTDeserializer deserializer(argv[1]);
  lcpl::ProgramPtr program = deserializer.getRootNode();

  try {
    lcpl::SemanticAnalysis semanticAnalysis(program.get());
    semanticAnalysis.runAnalysis();

    if ((argc == 3) && (strcmp(argv[2], "--run") == 0)) {
      std::cout << "Running interpreter" << std::endl;
      semanticAnalysis.runInterpreter();
    }
  } catch (lcpl::SemanticException &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}

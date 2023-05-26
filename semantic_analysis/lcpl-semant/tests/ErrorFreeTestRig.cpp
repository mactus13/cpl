#include <memory>
#include <string>

#include "SemanticException.h"
#include "TestRig.h"

std::unique_ptr<Test> getTest() {
  class ErrorFreeTest : public Test {
    bool test(lcpl::Program *p) noexcept override {
      try {
        createAndRunSema(p);
      } catch (std::exception &e) {
        return fail(std::string("Unexpected error: ") + e.what());
      }

      return pass();
    }
  };

  return std::unique_ptr<Test>(new ErrorFreeTest);
}

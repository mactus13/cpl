#include "SemanticException.h"
#include "TestRig.h"

std::unique_ptr<Test> getTest() {
  class MyTest : public Test {
    bool test(lcpl::Program *p) noexcept override {
      try {
        createAndRunSema(p);
      } catch (lcpl::TooManyArgsException &) {
        return pass();
      } catch (lcpl::SemanticException &e) {
        return fail(std::string("Wrong error: ") + e.what());
      }

      return fail("Should error");
    }
  };

  return std::unique_ptr<Test>(new MyTest);
}

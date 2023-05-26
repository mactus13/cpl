#ifndef TESTRIG_H
#define TESTRIG_H

#include <memory>

namespace lcpl {
class Program;
}

/// \brief Base class for sema tests
///
/// Each test should subclass this and override the \c test method. It should
/// also provide an implementation of the \c getTest factory function below.
class Test {
public:
  virtual ~Test();

  /// \brief Run the current test on the given AST
  /// \return true if the test passed and false otherwise
  bool run(const std::string &pathToAST);

protected:
  void createAndRunSema(lcpl::Program *program);

  bool pass();
  bool fail(const std::string &message);

private:
  /// \brief Catch any semantic exceptions and return true or false depending on
  /// whether or not that was the exception we expected
  virtual bool test(lcpl::Program *program) noexcept = 0;
};

/// \brief Factory function that returns a subclass of \c Test
std::unique_ptr<Test> getTest();

#endif /* TESTRIG_H */

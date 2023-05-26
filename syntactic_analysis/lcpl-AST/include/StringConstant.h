#ifndef STRINGCONSTANT_H_
#define STRINGCONSTANT_H_

#include "Expression.h"

namespace lcpl {
/// \brief AST node for a string constant
class StringConstant : public Expression {

public:
  explicit StringConstant(int lineNumber, const std::string &value)
      : Expression(lineNumber), value(value) {}

  std::string getValue() const { return value; }

private:
  std::string value;
};

typedef std::unique_ptr<StringConstant> StringConstantPtr;

}
#endif /* STRINGCONSTANT_H_ */

#ifndef VOIDCONSTANT_H
#define VOIDCONSTANT_H

#include "Expression.h"

namespace lcpl {
/// \brief AST node for the null constant
class NullConstant : public Expression {
public:
  NullConstant(int lineNumber) : Expression(lineNumber) {}
};

typedef std::unique_ptr<NullConstant> NullConstantPtr;

}
#endif /* VOIDCONSTANT_H */

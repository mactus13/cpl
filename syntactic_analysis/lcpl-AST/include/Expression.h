#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "TreeNode.h"
#include <string>

namespace lcpl {
/// \brief AST node for an expression
///
/// This is the base class for all expressions in LCPL
class Expression : public TreeNode {
public:
  explicit Expression(int lineNumber) : TreeNode(lineNumber) {}

  void setType(const std::string &type) { this->type = type; }

  std::string getType() const { return type; }

private:
  std::string type;
};

typedef std::unique_ptr<Expression> ExpressionPtr;

}
#endif /* EXPRESSION_H_ */

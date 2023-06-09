#ifndef BLOCK_H
#define BLOCK_H

#include <memory>
#include <utility>
#include <vector>

#include "Expression.h"

#include "support/iterator.h"

namespace lcpl {
/// \brief AST node for a block of statements
class Block : public Expression {
  typedef std::vector<std::unique_ptr<Expression>> ExpressionsType;

  struct iterator
      : public llvm::iterator_adaptor_base<iterator,
                                           ExpressionsType::const_iterator> {
    explicit iterator(ExpressionsType::const_iterator &&wrapped)
        : iterator_adaptor_base(wrapped) {}
    Expression *operator*() const { return I->get(); }
  };

public:
  /// \brief Create a block node
  /// \note This will take ownership of all the nodes in \p expressions, if
  ///       provided
  explicit Block(int lineNumber,
                 const std::vector<Expression *> &expressions = {})
      : Expression(lineNumber), expressions() {
    for (auto expr : expressions) {
      this->expressions.emplace_back(expr);
    }
  }

  /// \brief Add expression and take ownership of it
  void addExpression(std::unique_ptr<Expression> E) {
    expressions.push_back(std::move(E));
  }

  /// @{
  /// \brief Support for iterating through the expressions in this block
  iterator begin() const { return iterator(expressions.begin()); }
  iterator end() const { return iterator(expressions.end()); }
  /// @}

private:
  ExpressionsType expressions;
};

typedef std::unique_ptr<Block> BlockPtr;

}
#endif /* BLOCK_H */

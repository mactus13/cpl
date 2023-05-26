#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <memory>
#include <utility>

#include "Feature.h"
#include "Expression.h"

namespace lcpl {

/// \brief AST node for a class attribute
class Attribute : public Feature {
public:
  /// \brief Create an Attribute node
  /// \note This will take ownership of \p init, if provided
  explicit Attribute(int lineNumber, const std::string &name,
                     const std::string &type,
                     std::unique_ptr<Expression> init = nullptr)
      : Feature(lineNumber, name), type(type), init(std::move(init)) {}

  void setInit(std::unique_ptr<Expression> E) { init = std::move(E); }

  std::string getType() const { return type; }
  Expression *getInit() const { return init.get(); }

private:
  std::string type;
  std::unique_ptr<Expression> init;
};

typedef std::unique_ptr<Attribute> AttributePtr;

}
#endif /* ATTRIBUTE_H_ */

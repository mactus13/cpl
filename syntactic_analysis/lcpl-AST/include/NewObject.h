#ifndef NEWOBJECT_H
#define NEWOBJECT_H

#include <string>

#include "Expression.h"

namespace lcpl {
/// \brief AST node for creating a new object
class NewObject : public Expression {
public:
  explicit NewObject(int lineNumber, std::string type)
      : Expression(lineNumber), type(type) {}

  std::string getType() const { return type; }

private:
  std::string type;
};

typedef std::unique_ptr<NewObject> NewObjectPtr;

}
#endif /* NEWOBJECT_H */

#ifndef FEATURE_H_
#define FEATURE_H_

#include <string>

#include "TreeNode.h"

namespace lcpl {
/// \brief AST node for a class feature
///
/// Base class for class features (methods and attributes)
class Feature : public TreeNode {
public:
  explicit Feature(int lineNumber, const std::string &name)
      : TreeNode(lineNumber), name(name) {}

  std::string getName() const { return name; }

private:
  std::string name;
};

typedef std::unique_ptr<Feature> FeaturePtr;

}
#endif /* FEATURE_H_ */

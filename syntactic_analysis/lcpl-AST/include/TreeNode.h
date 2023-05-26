#ifndef TREENODE_H_
#define TREENODE_H_

#include <memory>

namespace lcpl {
/// \brief Base class for all AST nodes
class TreeNode {
  TreeNode(const TreeNode &) = delete;
  TreeNode(TreeNode &&) = delete;
  TreeNode &operator=(const TreeNode &) = delete;
  TreeNode &operator=(TreeNode &&) = delete;

public:
  explicit TreeNode(int lineNumber) : lineNumber(lineNumber) {}
  virtual ~TreeNode() {}

  int getLineNumber() const { return lineNumber; }

private:
  int lineNumber;
};

typedef std::unique_ptr<TreeNode> TreeNodePtr;
}
#endif /* TREENODE_H_ */


#ifndef INCLUDE_SYMBOLTABLE_H_
#define INCLUDE_SYMBOLTABLE_H_

#include <cassert>
#include <vector>
#include <string>
#include <unordered_map>

namespace lcpl {
class TreeNode;

class SymbolTable {
  typedef std::unordered_map<std::string, TreeNode *> SymbolTableScope;

public:
  SymbolTable() {}
  ~SymbolTable() {}

  // RAII class for scope management
  struct Scope {
    Scope(SymbolTable &s) : symbolTable(s) { symbolTable.pushScope(); }
    ~Scope() { symbolTable.popScope(); }

  private:
    SymbolTable &symbolTable;
  };

  template <typename NamedNode> void insert(NamedNode *v) {
    assert(!symbolTable.empty() && "insert on an empty symbol table");
    insert(v, v->getName());
  }

  TreeNode *lookup(const std::string &name) const;

private:
  void insert(TreeNode *v, const std::string &name);

  void pushScope();
  void popScope();

  SymbolTableScope *getScope(const std::string &name) const;

  std::vector<SymbolTableScope *> symbolTable;
};
}

#endif /* INCLUDE_SYMBOLTABLE_H_ */

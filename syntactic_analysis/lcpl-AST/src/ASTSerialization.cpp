#include "ASTSerialization.h"

#include <cassert>
#include <iostream>

#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"

using namespace lcpl;

namespace strings {
const auto NodeType = "NodeType";
const auto NodeID = "NodeID";
const auto LineNumber = "LineNumber";
const auto Classes = "Classes";
const auto Name = "Name";
const auto ClassParent = "Parent";
const auto Features = "Features";
const auto Type = "Type";
const auto Initializer = "Initializer";
const auto ReturnType = "ReturnType";
const auto Body = "Body";
const auto FormalParams = "FormalParams";
const auto Value = "Value";
const auto Expressions = "Expressions";
const auto OpKind = "OperatorKind";
const auto LHS = "LeftHandSide";
const auto RHS = "RightHandSide";
const auto Operand = "Operand";
const auto Object = "Object";
const auto Start = "Start";
const auto End = "End";
const auto Arguments = "Arguments";
const auto Condition = "Condition";
const auto Then = "Then";
const auto Else = "Else";
const auto Scope = "Scope";

const auto ProgramNodeType = "Program";
const auto ClassNodeType = "Class";
const auto AttributeNodeType = "Attribute";
const auto MethodNodeType = "Method";
const auto FormalParamNodeType = "FormalParam";
const auto IntConstantNodeType = "IntegerConstant";
const auto StringConstantNodeType = "StringConstant";
const auto NullConstantNodeType = "NullConstant";
const auto BlockNodeType = "Block";
const auto AssignmentNodeType = "Assignment";
const auto BinaryOperatorNodeType = "BinaryOperator";
const auto UnaryOperatorNodeType = "UnaryOperator";
const auto CastNodeType = "Cast";
const auto SubstringNodeType = "Substring";
const auto DispatchNodeType = "Dispatch";
const auto StaticDispatchNodeType = "StaticDispatch";
const auto NewObjectNodeType = "NewObject";
const auto IfStatementNodeType = "IfStatement";
const auto WhileStatementNodeType = "WhileStatement";
const auto LocalDefinitionNodeType = "LocalDefinition";
const auto SymbolNodeType = "Symbol";
}

ASTSerializer::ASTSerializer(const std::string &filename)
    : outputFile(std::fopen(filename.c_str(), "w")) {
  if (outputFile) {
    outputStream.reset(
        new FileStream(outputFile, writeBuffer, sizeof(writeBuffer)));
    writer.reset(new Writer(*outputStream));
  } else {
    std::cerr << "Error opening output file " << filename << std::endl;
  }
}

ASTSerializer::~ASTSerializer() {
  if (outputFile) {
    if (std::fclose(outputFile)) {
      std::cerr << "Error closing output file" << std::endl;
    }
  }
}

struct ASTSerializer::CreateJSONObject {
  CreateJSONObject(ASTSerializer &S, const ASTSerializer::Writer::Ch *nodeType,
                   TreeNode *Node)
      : parent(S) {
    parent.writer->StartObject();
    parent.writePair(strings::NodeType, nodeType);
    parent.writePair(strings::NodeID, parent.newID(Node));
  }
  ~CreateJSONObject() { parent.writer->EndObject(); }

private:
  ASTSerializer &parent;
};

struct ASTSerializer::CreateJSONArray {
  CreateJSONArray(ASTSerializer &S) : parent(S) { parent.writer->StartArray(); }
  ~CreateJSONArray() { parent.writer->EndArray(); }

private:
  ASTSerializer &parent;
};

void ASTSerializer::writePair(const Writer::Ch *key, const Writer::Ch *value) {
  // TODO: check for errors
  writer->Key(key);
  writer->String(value);
}

void ASTSerializer::writePair(const Writer::Ch *key, int value) {
  writer->Key(key);
  writer->Int(value);
}

bool ASTSerializer::visit(Program *P) {
  assert(isValid() && "Invalid serializer");
  assert(P && "Expected non-null program");

  CreateJSONObject object(*this, strings::ProgramNodeType, P);
  writePair(strings::LineNumber, P->getLineNumber());

  writer->Key(strings::Classes);
  if (P->begin() == P->end()) {
    return true;
  }

  CreateJSONArray array(*this);
  return ASTVisitor::visit(P);
}

bool ASTSerializer::visit(Class *C) {
  assert(isValid() && "Invalid serializer");
  assert(C && "Expected non-null class");

  CreateJSONObject object(*this, strings::ClassNodeType, C);
  writePair(strings::LineNumber, C->getLineNumber());
  writePair(strings::Name, C->getName());

  auto parent = C->getParent();
  if (!parent.empty()) {
    writePair(strings::ClassParent, parent);
  }

  if (C->begin() == C->end()) {
    return true;
  }

  writer->Key(strings::Features);
  CreateJSONArray array(*this);

  return ASTVisitor::visit(C);
}

bool ASTSerializer::visit(Attribute *A) {
  assert(isValid() && "Invalid serializer");
  assert(A && "Expected non-null attribute");

  CreateJSONObject attrObject(*this, strings::AttributeNodeType, A);
  writePair(strings::LineNumber, A->getLineNumber());
  writePair(strings::Name, A->getName());
  writePair(strings::Type, A->getType());

  if (auto Init = A->getInit()) {
    writer->Key(strings::Initializer);
    if (!visit(Init)) {
      return false;
    }
  }

  return true;
}

bool ASTSerializer::visit(Method *M) {
  assert(isValid() && "Invalid serializer");
  assert(M && "Expected non-null method");

  CreateJSONObject methodObject(*this, "Method", M);
  writePair(strings::LineNumber, M->getLineNumber());
  writePair(strings::Name, M->getName());

  auto ret = M->getReturnType();
  if (!ret.empty()) {
    writePair(strings::ReturnType, ret);
  }

  if (auto body = M->getBody()) {
	writer->Key(strings::Body);
    if (!visit(body)) {
      return false;
    }
  }

  if (M->begin() == M->end()) {
    return true;
  }

  writer->Key(strings::FormalParams);
  CreateJSONArray paramArray(*this);
  for (auto Param : *M) {
    if (!visit(Param)) {
      return false;
    }
  }

  return true;
}

bool ASTSerializer::visit(FormalParam *F) {
  assert(isValid() && "Invalid serializer");
  assert(F && "Expected non-null formal parameter");

  CreateJSONObject object(*this, strings::FormalParamNodeType, F);
  writePair(strings::LineNumber, F->getLineNumber());
  writePair(strings::Name, F->getName());
  writePair(strings::Type, F->getType());

  return true;
}

bool ASTSerializer::visit(Expression *E) {
  assert(isValid() && "Invalid serializer");
  assert(E && "Expected non-null expression");
  return ASTVisitor::visit(E);
}

bool ASTSerializer::visit(IntConstant *IC) {
  assert(isValid() && "Invalid serializer");
  assert(IC && "Expected non-null integer constant");

  CreateJSONObject object(*this, strings::IntConstantNodeType, IC);
  writePair(strings::LineNumber, IC->getLineNumber());
  writePair(strings::Value, IC->getValue());

  return true;
}

bool ASTSerializer::visit(StringConstant *SC) {
  assert(isValid() && "Invalid serializer");
  assert(SC && "Expected non-null string constant");

  CreateJSONObject object(*this, strings::StringConstantNodeType, SC);
  writePair(strings::LineNumber, SC->getLineNumber());
  writePair(strings::Value, SC->getValue());

  return true;
}

bool ASTSerializer::visit(NullConstant *NC) {
  assert(isValid() && "Invalid serializer");
  assert(NC && "Expected non-null null constant :)");

  CreateJSONObject object(*this, strings::NullConstantNodeType, NC);
  writePair(strings::LineNumber, NC->getLineNumber());

  return true;
}

bool ASTSerializer::visit(Block *B) {
  assert(isValid() && "Invalid serializer");
  assert(B && "Expected non-null block");

  CreateJSONObject object(*this, strings::BlockNodeType, B);
  writePair(strings::LineNumber, B->getLineNumber());

  if (B->begin() == B->end()) {
    return true;
  }

  writer->Key(strings::Expressions);
  CreateJSONArray expressions(*this);
  return ASTVisitor::visit(B);
}

bool ASTSerializer::visit(Assignment* A) {
	assert(isValid() && "Invalid serializer");
	assert(A && "Expected non-null assignment");

	CreateJSONObject object(*this, strings::AssignmentNodeType, A);
	writePair(strings::LineNumber, A->getLineNumber());
	writePair(strings::LHS, A->getSymbol());

	writer->Key(strings::RHS);
	auto RHS = A->getExpression();
	assert(RHS && "Assignment does not have an expression to assign");
	if(!visit(RHS)) {
		return false;
	}
	return true;
}

static std::string getBinOpKindSerialization(BinaryOperator::BinOpKind opKind) {
  switch (opKind) {
  case BinaryOperator::Add:
    return "+";
  case BinaryOperator::Sub:
    return "-";
  case BinaryOperator::Mul:
    return "*";
  case BinaryOperator::Div:
    return "/";
  case BinaryOperator::LessThan:
    return "<";
  case BinaryOperator::LessThanEqual:
    return "<=";
  case BinaryOperator::Equal:
    return "==";
  default:
    assert(false && "Unhandled binary operator");
  }
}

bool ASTSerializer::visit(BinaryOperator *BO) {
  assert(isValid() && "Invalid serializer");
  assert(BO && "Expected non-null binary operator");

  CreateJSONObject opObject(*this, strings::BinaryOperatorNodeType, BO);
  writePair(strings::LineNumber, BO->getLineNumber());

  std::string opKind = getBinOpKindSerialization(BO->getOperatorKind());
  writePair(strings::OpKind, opKind);

  writer->Key(strings::LHS);
  auto LHS = BO->getLHS();
  assert(LHS && "Binary operator doesn't have LHS");
  if (!visit(LHS)) {
    return false;
  }

  writer->Key(strings::RHS);
  auto RHS = BO->getRHS();
  assert(RHS && "Binary operator doesn't have RHS");
  if (!visit(RHS)) {
    return false;
  }

  return true;
}

bool ASTSerializer::visit(UnaryOperator *UO) {
  assert(isValid() && "Invalid serializer");
  assert(UO && "Expected non-null unary operator");

  CreateJSONObject opObject(*this, strings::UnaryOperatorNodeType, UO);
  writePair(strings::LineNumber, UO->getLineNumber());

  std::string opKind;
  switch (UO->getOperatorKind()) {
  case UnaryOperator::Minus:
    opKind = "-";
    break;
  case UnaryOperator::Not:
    opKind = "!";
    break;
  default:
    assert(false && "Unhandled unary operator");
  }

  writePair(strings::OpKind, opKind);

  writer->Key(strings::Operand);
  auto operand = UO->getOperand();
  assert(operand && "Unary operator doesn't have operand");
  return visit(operand);
}

bool ASTSerializer::visit(Cast *C) {
  assert(isValid() && "Invalid serializer");
  assert(C && "Expected non-null cast");

  CreateJSONObject castObject(*this, strings::CastNodeType, C);
  writePair(strings::LineNumber, C->getLineNumber());
  writePair(strings::Type, C->getType());

  writer->Key(strings::Operand);
  auto ToCast = C->getExpressionToCast();
  assert(ToCast && "Unary operator doesn't have operand");
  return visit(ToCast);
}

bool ASTSerializer::visit(Substring *S) {
  assert(isValid() && "Invalid serializer");
  assert(S && "Expected non-null substring operator");

  CreateJSONObject substringObject(*this, strings::SubstringNodeType, S);
  writePair(strings::LineNumber, S->getLineNumber());

  if (auto strObject = S->getString()) {
    writer->Key(strings::Object);
    assert(strObject && "Substring operator doesn't have string object");
    if (!visit(strObject)) {
      return false;
    }
  }

  writer->Key(strings::Start);
  auto start = S->getStart();
  assert(start && "Substring operator doesn't have start index");
  if (!visit(start)) {
    return false;
  }

  writer->Key(strings::End);
  auto end = S->getEnd();
  assert(end && "Substring operator doesn't have end index");
  if (!visit(end)) {
    return false;
  }

  return true;
}

bool ASTSerializer::visit(Dispatch *D) {
  assert(isValid() && "Invalid serializer");
  assert(D && "Expected non-null dispatch");

  CreateJSONObject dispatchObject(*this, strings::DispatchNodeType, D);
  writePair(strings::LineNumber, D->getLineNumber());
  writePair(strings::Name, D->getName());

  if (auto object = D->getObject()) {
    writer->Key(strings::Object);
    assert(object && "Dispatch doesn't have object");
    if (!visit(object)) {
      return false;
    }
  }

  if (D->begin() == D->end()) {
    return true;
  }

  writer->Key(strings::Arguments);
  CreateJSONArray arguments(*this);
  for (auto arg : *D) {
    if (!visit(arg)) {
      return false;
    }
  }

  return true;
}

bool ASTSerializer::visit(StaticDispatch *SD) {
  assert(isValid() && "Invalid serializer");
  assert(SD && "Expected non-null static dispatch");

  CreateJSONObject dispatchObject(*this, strings::StaticDispatchNodeType, SD);
  writePair(strings::LineNumber, SD->getLineNumber());
  writePair(strings::Type, SD->getType());
  writePair(strings::Name, SD->getName());

  if (auto object = SD->getObject()) {
    writer->Key(strings::Object);
    assert(object && "Static dispatch doesn't have object");
    if (!visit(object)) {
      return false;
    }
  }

  if (SD->begin() == SD->end()) {
    return true;
  }

  writer->Key(strings::Arguments);
  CreateJSONArray arguments(*this);
  for (auto arg : *SD) {
    if (!visit(arg)) {
      return false;
    }
  }

  return true;
}

bool ASTSerializer::visit(NewObject *NO) {
  assert(isValid() && "Invalid serializer");
  assert(NO && "Expected non-null new operator");

  CreateJSONObject newObject(*this, strings::NewObjectNodeType, NO);
  writePair(strings::LineNumber, NO->getLineNumber());
  writePair(strings::Type, NO->getType());
  return true;
}

bool ASTSerializer::visit(IfStatement *If) {
  assert(isValid() && "Invalid serializer");
  assert(If && "Expected non-null if statement");

  CreateJSONObject ifObject(*this, strings::IfStatementNodeType, If);
  writePair(strings::LineNumber, If->getLineNumber());

  writer->Key(strings::Condition);
  auto condExpr = If->getCond();
  assert(condExpr && "If doesn't have condition");
  if (!visit(condExpr)) {
    return false;
  }

  writer->Key(strings::Then);
  auto thenExpr = If->getThen();
  assert(thenExpr && "If doesn't have then branch");
  if (!visit(thenExpr)) {
    return false;
  }

  if (auto elseExpr = If->getElse()) {
    writer->Key(strings::Else);
    if (!visit(elseExpr)) {
      return false;
    }
  }

  return true;
}

bool ASTSerializer::visit(WhileStatement *While) {
  assert(isValid() && "Invalid serializer");
  assert(While && "Expected non-null while statement");

  CreateJSONObject whileObject(*this, strings::WhileStatementNodeType, While);
  writePair(strings::LineNumber, While->getLineNumber());

  writer->Key(strings::Condition);
  auto cond = While->getCond();
  assert(cond && "While doesn't have condition");
  if (!visit(cond)) {
    return false;
  }

  writer->Key(strings::Body);
  auto body = While->getBody();
  assert(body && "While doesn't have body");
  return visit(body);
}

bool ASTSerializer::visit(LocalDefinition *Local) {
  assert(isValid() && "Invalid serializer");
  assert(Local && "Expected non-null local definition");

  CreateJSONObject object(*this, strings::LocalDefinitionNodeType, Local);
  writePair(strings::LineNumber, Local->getLineNumber());
  writePair(strings::Name, Local->getName());
  writePair(strings::Type, Local->getType());

  if(auto Scope = Local->getScope()) {
	  // TODO: assert(Scope && "Local definition doesn't have scope"); set scope during syntax analysis
	  writePair(strings::Scope, getID(Scope));
  }

  if (auto Init = Local->getInit()) {
    writer->Key(strings::Initializer);
    if (!visit(Init)) {
      return false;
    }
  }

  return true;
}

bool ASTSerializer::visit(Symbol *Sym) {
  assert(isValid() && "Invalid serializer");
  assert(Sym && "Expected non-null symbol");

  CreateJSONObject object(*this, strings::SymbolNodeType, Sym);
  writePair(strings::LineNumber, Sym->getLineNumber());
  writePair(strings::Name, Sym->getName());
  return true;
}

ASTDeserializer::ASTDeserializer(const std::string &filename)
    : inputFile(std::fopen(filename.c_str(), "r")) {
  if (inputFile) {
    inputStream.reset(
        new FileStream(inputFile, readBuffer, sizeof(readBuffer)));
  } else {
    std::cerr << "Error opening input file " << filename << std::endl;
  }
}

ASTDeserializer::~ASTDeserializer() {
  if (inputFile) {
    if (std::fclose(inputFile)) {
      std::cerr << "Error closing input file" << std::endl;
    }
  }
}

std::unique_ptr<Program> ASTDeserializer::getRootNode() {
  assert(isValid() && "Invalid deserializer");

  rapidjson::Document tree;
  (void)tree.ParseStream(*inputStream);

  if (tree.HasParseError()) {
    std::cerr << "Failed to parse input JSON: "
              << rapidjson::GetParseError_En(tree.GetParseError()) << std::endl;
    return nullptr;
  }

  assert(tree.IsObject() && "Expected program object");
  return parseProgram(tree);
}

void ASTDeserializer::parseID(rapidjson::Value &tree, TreeNode *node) {
  assert(tree.HasMember(strings::NodeID) && "Nodes must have ids");
  assert(tree[strings::NodeID].IsUint() && "Invalid node id");

  addNode(tree[strings::NodeID].GetUint(), node);
}

int ASTDeserializer::parseLineNumber(rapidjson::Value &tree) {
  assert(tree.HasMember(strings::LineNumber) && "Nodes must have line numbers");

  auto &lineNumberTree = tree[strings::LineNumber];
  assert(lineNumberTree.IsInt() && "Invalid line number");

  return lineNumberTree.GetInt();
}

std::unique_ptr<Program> ASTDeserializer::parseProgram(rapidjson::Value &tree) {
  assert(tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::ProgramNodeType &&
         "Expected program object");

  auto program = createNode<Program>(tree, parseLineNumber(tree));

  if (tree.HasMember(strings::Classes)) {
    auto &classes = tree[strings::Classes];
    assert(classes.IsArray() && "Classes should live in an array");
    for (auto b = classes.Begin(), e = classes.End(); b != e; ++b) {
      auto &classTree = *b;
      assert(classTree.IsObject() && "Expected class object");
      auto classNode = parseClass(classTree);
      assert(classNode && "Expected non-null class");
      program->addClass(std::move(classNode));
    }
  }

  return program;
}

std::unique_ptr<Class> ASTDeserializer::parseClass(rapidjson::Value &tree) {
  assert(tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::ClassNodeType &&
         "Expected class object");

  assert(tree.HasMember(strings::Name) && "Class without name");

  std::string parent;
  if (tree.HasMember(strings::ClassParent)) {
    assert(tree[strings::ClassParent].IsString() && "Invalid parent for class");
    parent = tree[strings::ClassParent].GetString();
  }

  auto classNode = createNode<Class>(tree, parseLineNumber(tree),
                                     tree[strings::Name].GetString(), parent);

  if (tree.HasMember(strings::Features)) {
    auto &features = tree[strings::Features];
    assert(features.IsArray() && "Features must be in an array");
    for (auto b = features.Begin(), e = features.End(); b != e; ++b) {
      auto &featureTree = *b;
      assert(featureTree.IsObject() && "Expected feature object");
      auto featureNode = parseFeature(featureTree);
      assert(featureNode && "Expected non-null feature");
      classNode->addFeature(std::move(featureNode));
    }
  }

  return classNode;
}

std::unique_ptr<Feature> ASTDeserializer::parseFeature(rapidjson::Value &tree) {
  assert(tree.HasMember(strings::NodeType) && "Expected feature object");
  if (tree[strings::NodeType] == strings::AttributeNodeType) {
    return parseAttribute(tree);
  }

  if (tree[strings::NodeType] == strings::MethodNodeType) {
    return parseMethod(tree);
  }

  assert(false && "Unknown feature type");
  return nullptr;
}

std::unique_ptr<Attribute>
ASTDeserializer::parseAttribute(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::AttributeNodeType &&
         "Expected attribute object");

  assert(tree.HasMember(strings::Name) && "Attribute without name");
  assert(tree[strings::Name].IsString() && "Invalid attribute name");

  assert(tree.HasMember(strings::Type) && "Attribute without type");
  assert(tree[strings::Type].IsString() && "Invalid attribute type");
  auto attribute = createNode<Attribute>(tree, parseLineNumber(tree),
                                         tree[strings::Name].GetString(),
                                         tree[strings::Type].GetString());

  if (tree.HasMember(strings::Initializer)) {
    auto init = parseExpression(tree[strings::Initializer]);
    assert(init && "Expected non-null initializer");
    attribute->setInit(std::move(init));
  }

  return attribute;
}

std::unique_ptr<Method> ASTDeserializer::parseMethod(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::MethodNodeType &&
         "Expected method object");

  assert(tree.HasMember(strings::Name) && "Method without name");
  assert(tree[strings::Name].IsString() && "Invalid method name");

  std::unique_ptr<Expression> body(nullptr);
  if (tree.HasMember(strings::Body)) {
    assert(tree[strings::Body].IsObject() && "Invalid method body");

    body = parseExpression(tree[strings::Body]);
    assert(body && "Expected non-null method body");
  }

  std::string returnType;
  if (tree.HasMember(strings::ReturnType)) {
    assert(tree[strings::ReturnType].IsString() &&
           "Invalid method return type");
    returnType = tree[strings::ReturnType].GetString();
  }

  auto method = createNode<Method>(tree, parseLineNumber(tree),
                                   tree[strings::Name].GetString(),
                                   returnType, std::move(body));

  if (tree.HasMember(strings::FormalParams)) {
    auto &formalParams = tree[strings::FormalParams];
    assert(formalParams.IsArray() && "Formal params must be in an array");
    for (auto b = formalParams.Begin(), e = formalParams.End(); b != e; ++b) {
      auto &formalParamTree = *b;
      assert(formalParamTree.IsObject() && "Expected formal param object");
      auto formalParamNode = parseFormalParam(formalParamTree);
      assert(formalParamNode && "Expected non-null formal param");
      method->addParameter(std::move(formalParamNode));
    }
  }

  return method;
}

std::unique_ptr<FormalParam>
ASTDeserializer::parseFormalParam(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::FormalParamNodeType &&
         "Expected formal param object");

  assert(tree.HasMember(strings::Name) && "Formal param without name");
  assert(tree[strings::Name].IsString() && "Invalid formal param name");

  assert(tree.HasMember(strings::Type) && "Formal param without type");
  assert(tree[strings::Type].IsString() && "Invalid formal param type");

  return createNode<FormalParam>(tree, parseLineNumber(tree),
                                 tree[strings::Name].GetString(),
                                 tree[strings::Type].GetString());
}

std::unique_ptr<Expression>
ASTDeserializer::parseExpression(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         "Expected expression object");

  auto &nodeType = tree[strings::NodeType];
  if (nodeType == strings::IntConstantNodeType) {
    return parseIntConstant(tree);
  } else if (nodeType == strings::StringConstantNodeType) {
    return parseStringConstant(tree);
  } else if (nodeType == strings::NullConstantNodeType) {
    return parseNullConstant(tree);
  } else if (nodeType == strings::SymbolNodeType) {
    return parseSymbol(tree);
  } else if (nodeType == strings::BlockNodeType) {
    return parseBlock(tree);
  } else if (nodeType == strings::BinaryOperatorNodeType) {
    return parseBinOp(tree);
  } else if (nodeType == strings::UnaryOperatorNodeType) {
    return parseUnaryOp(tree);
  } else if (nodeType == strings::CastNodeType) {
    return parseCast(tree);
  } else if (nodeType == strings::SubstringNodeType) {
    return parseSubstring(tree);
  } else if (nodeType == strings::DispatchNodeType) {
    return parseDispatch(tree);
  } else if (nodeType == strings::StaticDispatchNodeType) {
    return parseStaticDispatch(tree);
  } else if (nodeType == strings::NewObjectNodeType) {
    return parseNewObject(tree);
  } else if (nodeType == strings::IfStatementNodeType) {
    return parseIfStatement(tree);
  } else if (nodeType == strings::WhileStatementNodeType) {
    return parseWhileStatement(tree);
  } else if (nodeType == strings::LocalDefinitionNodeType) {
    return parseLocalDefinition(tree);
  } else if (nodeType == strings::AssignmentNodeType) {
	  return parseAssignment(tree);
  }

  assert(false && "Unknown expression kind");
  return nullptr;
}

std::unique_ptr<Assignment>
ASTDeserializer::parseAssignment(rapidjson::Value& tree) {
	assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
		tree[strings::NodeType] == strings::AssignmentNodeType &&
		"Expected assignment object");

	assert(tree.HasMember(strings::LHS) && "Assignment without LHS");
	assert(tree[strings::LHS].IsString() && "Invalid LHS for assignment");

	assert(tree.HasMember(strings::RHS) && "Assignment without RHS");
	assert(tree[strings::RHS].IsObject() && "Invalid RHS for assignment");

	auto RHS = parseExpression(tree[strings::RHS]);
	assert(RHS && "Expected non-null expression node");

	return createNode<Assignment>(tree, parseLineNumber(tree),
									tree[strings::LHS].GetString(),
									std::move(RHS));
}

std::unique_ptr<IntConstant>
ASTDeserializer::parseIntConstant(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::IntConstantNodeType &&
         "Expected int constant object");

  assert(tree.HasMember(strings::Value) && "Int constant without value");
  assert(tree[strings::Value].IsInt() && "Invalid value for int constant");
  // TODO: figure out about unsigned etc

  return createNode<IntConstant>(tree, parseLineNumber(tree),
                                 tree[strings::Value].GetInt());
}

std::unique_ptr<StringConstant>
ASTDeserializer::parseStringConstant(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::StringConstantNodeType &&
         "Expected string constant object");

  assert(tree.HasMember(strings::Value) && "String constant without value");
  assert(tree[strings::Value].IsString() &&
         "Invalid value for string constant");

  return createNode<StringConstant>(tree, parseLineNumber(tree),
                                    tree[strings::Value].GetString());
}

std::unique_ptr<NullConstant>
ASTDeserializer::parseNullConstant(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::NullConstantNodeType &&
         "Expected null constant object");
  return createNode<NullConstant>(tree, parseLineNumber(tree));
}

std::unique_ptr<Symbol> ASTDeserializer::parseSymbol(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::SymbolNodeType &&
         "Expected symbol object");

  assert(tree.HasMember(strings::Name) && "Symbol without name");
  assert(tree[strings::Name].IsString() && "Invalid symbol name");

  return createNode<Symbol>(tree, parseLineNumber(tree),
                            tree[strings::Name].GetString());
}

std::unique_ptr<Block> ASTDeserializer::parseBlock(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::BlockNodeType &&
         "Expected block object");

  auto block = createNode<Block>(tree, parseLineNumber(tree));

  if (tree.HasMember(strings::Expressions)) {
    assert(tree[strings::Expressions].IsArray() && "Expressions not in array");

    auto &expressions = tree[strings::Expressions];
    for (auto b = expressions.Begin(), e = expressions.End(); b != e; ++b) {
      auto &expressionTree = *b;
      assert(expressionTree.IsObject() && "Expected expression object");
      auto expressionNode = parseExpression(expressionTree);
      assert(expressionNode && "Expected non-null expression node");
      block->addExpression(std::move(expressionNode));
    }
  }

  return block;
}

static BinaryOperator::BinOpKind getBinOpKind(rapidjson::Value &tree) {
  assert(tree.HasMember(strings::OpKind) && "Binary operator without op kind");

  auto &opKind = tree[strings::OpKind];
  assert(opKind.IsString() && "Invalid binary op kind");

  if (opKind == "+") {
    return BinaryOperator::Add;
  } else if (opKind == "-") {
    return BinaryOperator::Sub;
  } else if (opKind == "*") {
    return BinaryOperator::Mul;
  } else if (opKind == "/") {
    return BinaryOperator::Div;
  } else if (opKind == "<") {
    return BinaryOperator::LessThan;
  } else if (opKind == "<=") {
    return BinaryOperator::LessThanEqual;
  } else if (opKind == "==") {
    return BinaryOperator::Equal;
  }

  assert(false && "Unknown binary operator kind");
  return BinaryOperator::Invalid;
}

std::unique_ptr<BinaryOperator>
ASTDeserializer::parseBinOp(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::BinaryOperatorNodeType &&
         "Expected binary operator object");

  assert(tree.HasMember(strings::LHS) && "Binary operator without LHS");
  assert(tree[strings::LHS].IsObject() && "Invalid LHS for binary operator");

  auto LHS = parseExpression(tree[strings::LHS]);
  assert(LHS && "Expected non-null expression node");

  assert(tree.HasMember(strings::RHS) && "Binary operator without RHS");
  assert(tree[strings::RHS].IsObject() && "Invalid RHS for binary operator");

  auto RHS = parseExpression(tree[strings::RHS]);
  assert(RHS && "Expected non-null expression node");

  return createNode<BinaryOperator>(tree, parseLineNumber(tree),
                                    getBinOpKind(tree), std::move(LHS),
                                    std::move(RHS));
}

static UnaryOperator::UnaryOpKind getOpKind(rapidjson::Value &tree) {
  assert(tree.HasMember(strings::OpKind) && "Unary operator without op kind");
  assert(tree[strings::OpKind].IsString() &&
         "Invalid op kind for unary operator");

  auto &opKind = tree[strings::OpKind];
  if (opKind == "-") {
    return UnaryOperator::Minus;
  }

  if (opKind == "!") {
    return UnaryOperator::Not;
  }

  assert(false && "Unknown unary operator kind");
  return UnaryOperator::Invalid;
}

std::unique_ptr<UnaryOperator>
ASTDeserializer::parseUnaryOp(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::UnaryOperatorNodeType &&
         "Expected unary operator object");

  assert(tree.HasMember(strings::Operand) && "Unary operator without operand");
  assert(tree[strings::Operand].IsObject() &&
         "Invalid operand for unary operator");

  auto operand = parseExpression(tree[strings::Operand]);
  assert(operand && "Expected non-null expression node");

  return createNode<UnaryOperator>(tree, parseLineNumber(tree), getOpKind(tree),
                                   std::move(operand));
}

std::unique_ptr<Cast> ASTDeserializer::parseCast(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::CastNodeType &&
         "Expected cast object");

  assert(tree.HasMember(strings::Type) && "Cast without type");
  assert(tree[strings::Type].IsString() && "Invalid type for cast");

  auto type = tree[strings::Type].GetString();

  assert(tree.HasMember(strings::Operand) && "Cast without operand");
  assert(tree[strings::Operand].IsObject() && "Invalid operand for cast");

  auto operand = parseExpression(tree[strings::Operand]);
  assert(operand && "Expected non-null expression node");

  return createNode<Cast>(tree, parseLineNumber(tree), type,
                          std::move(operand));
}

std::unique_ptr<Substring>
ASTDeserializer::parseSubstring(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::SubstringNodeType &&
         "Expected substring object");

  std::unique_ptr<Expression> stringObject = nullptr;
  if (tree.HasMember(strings::Object)) {
    assert(tree[strings::Object].IsObject() &&
           "Invalid string object for substring");

    stringObject = parseExpression(tree[strings::Object]);
    assert(stringObject && "Expected non-null expression node");
  }

  assert(tree.HasMember(strings::Start) && "Substring without start");
  assert(tree[strings::Start].IsObject() && "Invalid start for substring");

  auto start = parseExpression(tree[strings::Start]);
  assert(start && "Expected non-null expression node");

  assert(tree.HasMember(strings::End) && "Substring without end");
  assert(tree[strings::End].IsObject() && "Invalid end for substring");

  auto end = parseExpression(tree[strings::End]);
  assert(end && "Expected non-null expression node");

  return createNode<Substring>(tree, parseLineNumber(tree),
                               std::move(stringObject), std::move(start),
                               std::move(end));
}

std::unique_ptr<Dispatch>
ASTDeserializer::parseDispatch(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::DispatchNodeType &&
         "Expected dispatch object");

  std::unique_ptr<Expression> object(nullptr);
  if (tree.HasMember(strings::Object)) {
    assert(tree[strings::Object].IsObject() && "Invalid object for dispatch");

    object = parseExpression(tree[strings::Object]);
    assert(object && "Expected non-null expression node");
  }

  assert(tree.HasMember(strings::Name) && "Dispatch without name");
  assert(tree[strings::Name].IsString() && "Invalid name for dispatch");

  auto dispatch =
      createNode<Dispatch>(tree, parseLineNumber(tree),
                           tree[strings::Name].GetString(), std::move(object));

  if (tree.HasMember(strings::Arguments)) {
    assert(tree[strings::Arguments].IsArray() && "Arguments not in array");

    auto &args = tree[strings::Arguments];
    for (auto b = args.Begin(), e = args.End(); b != e; ++b) {
      auto &argTree = *b;
      assert(argTree.IsObject() && "Expected argument object");
      auto argNode = parseExpression(argTree);
      assert(argNode && "Expected non-null expression node");
      dispatch->addArgument(std::move(argNode));
    }
  }

  return dispatch;
}

std::unique_ptr<StaticDispatch>
ASTDeserializer::parseStaticDispatch(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::StaticDispatchNodeType &&
         "Expected static dispatch object");

  std::unique_ptr<Expression> object(nullptr);
  if (tree.HasMember(strings::Object)) {
    assert(tree[strings::Object].IsObject() &&
           "Invalid object for static dispatch");

    object = parseExpression(tree[strings::Object]);
    assert(object && "Expected non-null expression node");
  }

  assert(tree.HasMember(strings::Name) && "Staic dispatch without name");
  assert(tree[strings::Name].IsString() && "Invalid name for static dispatch");

  assert(tree.HasMember(strings::Type) && "Static dispatch without type");
  assert(tree[strings::Type].IsString() && "Invalid type for static dispatch");

  auto dispatch = createNode<StaticDispatch>(
      tree, parseLineNumber(tree), std::move(object),
      tree[strings::Type].GetString(), tree[strings::Name].GetString());

  if (tree.HasMember(strings::Arguments)) {
    assert(tree[strings::Arguments].IsArray() && "Arguments not in array");

    auto &args = tree[strings::Arguments];
    for (auto b = args.Begin(), e = args.End(); b != e; ++b) {
      auto &argTree = *b;
      assert(argTree.IsObject() && "Expected argument object");
      auto argNode = parseExpression(argTree);
      assert(argNode && "Expected non-null expression node");
      dispatch->addArgument(std::move(argNode));
    }
  }

  return dispatch;
}

std::unique_ptr<NewObject>
ASTDeserializer::parseNewObject(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::NewObjectNodeType &&
         "Expected new operator object");

  assert(tree.HasMember(strings::Type) && "New operator without type");
  assert(tree[strings::Type].IsString() && "Invalid type for new operator");

  return createNode<NewObject>(tree, parseLineNumber(tree),
                               tree[strings::Type].GetString());
}

std::unique_ptr<IfStatement>
ASTDeserializer::parseIfStatement(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::IfStatementNodeType &&
         "Expected if statement object");

  assert(tree.HasMember(strings::Condition) &&
         "If statement without condition");
  assert(tree[strings::Condition].IsObject() &&
         "Invalid condition for if statement");

  auto cond = parseExpression(tree[strings::Condition]);
  assert(cond && "Expected non-null expression node");

  assert(tree.HasMember(strings::Then) && "If statement without then branch");
  assert(tree[strings::Then].IsObject() &&
         "Invalid then branch for if statement");

  auto then = parseExpression(tree[strings::Then]);
  assert(then && "Expected non-null expression node");

  std::unique_ptr<Expression> elseOrNull(nullptr);
  if (tree.HasMember(strings::Else)) {
    assert(tree[strings::Else].IsObject() &&
           "Invalid else branch for if statement");

    elseOrNull = parseExpression(tree[strings::Else]);
    assert(elseOrNull && "Expected non-null expression node");
  }

  return createNode<IfStatement>(tree, parseLineNumber(tree), std::move(cond),
                                 std::move(then), std::move(elseOrNull));
}

std::unique_ptr<WhileStatement>
ASTDeserializer::parseWhileStatement(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::WhileStatementNodeType &&
         "Expected while statement object");

  assert(tree.HasMember(strings::Condition) &&
         "While statement without condition");
  assert(tree[strings::Condition].IsObject() &&
         "Invalid condition for while statement");

  auto cond = parseExpression(tree[strings::Condition]);
  assert(cond && "Expected non-null expression node");

  assert(tree.HasMember(strings::Body) && "While statement without body");
  assert(tree[strings::Body].IsObject() && "Invalid body for while statement");

  auto body = parseExpression(tree[strings::Body]);
  assert(body && "Expected non-null expression node");

  return createNode<WhileStatement>(tree, parseLineNumber(tree),
                                    std::move(cond), std::move(body));
}

std::unique_ptr<LocalDefinition>
ASTDeserializer::parseLocalDefinition(rapidjson::Value &tree) {
  assert(tree.IsObject() && tree.HasMember(strings::NodeType) &&
         tree[strings::NodeType] == strings::LocalDefinitionNodeType &&
         "Expected while statement object");

  assert(tree.HasMember(strings::Name) && "Local definition without name");
  assert(tree[strings::Name].IsString() && "Invalid name for local definition");

  assert(tree.HasMember(strings::Type) && "Local definition without type");
  assert(tree[strings::Type].IsString() && "Invalid type for local definition");

  auto localDef = createNode<LocalDefinition>(tree, parseLineNumber(tree),
                                              tree[strings::Name].GetString(),
                                              tree[strings::Type].GetString());

// TODO: handle scope
//  assert(tree.HasMember(strings::Scope) && "Local definition without scope");
//  assert(tree[strings::Scope].IsInt() && "Invalid scope for local definition");

//  auto scopeNode = getNode(tree[strings::Scope].GetInt());
//  assert(scopeNode && "Unknown scope");
//
//  localDef->setScope(dynamic_cast<Expression *>(scopeNode));

  if (tree.HasMember(strings::Initializer)) {
    assert(tree[strings::Initializer].IsObject() &&
           "Invalid initializer for local definition");

    auto init = parseExpression(tree[strings::Initializer]);
    assert(init && "Expected non-null expression");

    localDef->setInit(std::move(init));
  }

  return localDef;
}

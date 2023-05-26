// Radu Vlad Rares - 341 C3

#include <cassert>
#include <iostream>
#include <unordered_set>

#include <ASTNodes.h>
#include <SemanticAnalysis.h>
#include <SemanticException.h>
#include <StringConstants.h>
#include <iostream>

using namespace lcpl;

SemanticAnalysis::SemanticAnalysis(Program *p)
    : program(p), typeTable(p), symbolTable() {
        assert(program && "Expected non-null program");
    }

void SemanticAnalysis::runAnalysis() {
    assert(program && "Expected non-null program");
    visit(program);
}

bool SemanticAnalysis::visit(Program *p) {
    checkMainClassAndMethod();
    checkInheritanceGraph();

    std::map<Class *, bool> processed;
    for (auto c : *p) {
        if (processed[c]) {
            throw DuplicateClassException(c);
        }

        processed[c] = true;

        if (!visit(c)) {
            return false;
        }
    }

    return true;
}

bool SemanticAnalysis::visit(Class *c) {
    if (typeTable.isBuiltinClass(c)) {
        return true;
    }

    // create a scope and add all attributes to it
    SymbolTable::Scope classScope(symbolTable);
    for (auto currentClass = c; currentClass;
         currentClass = typeTable.getParentClass(currentClass)) {
        for (auto f : *currentClass) {
            if (f->isAttribute()) {
                auto attr = static_cast<Attribute *>(f);
                symbolTable.insert(attr);

                auto attrType = typeTable.getType(attr->getType());
                typeTable.setType(attr, attrType);
            }
        }
    }

    // add self to symbol table
    std::unique_ptr<LocalDefinition> self(
                                          new LocalDefinition(0, "self", c->getName()));
    symbolTable.insert(self.get());
    typeTable.setType(self.get(), typeTable.getType(c));

    checkFeatures(c);

    return ASTVisitor::visit(c);
}

void SemanticAnalysis::checkFeatures(Class *c) {
    for (auto f : *c) {
        if (f->isAttribute()) {
            auto attr = static_cast<Attribute *>(f);
            if (typeTable.getAttribute(typeTable.getParentClass(c), f->getName())) {
                throw DuplicateAttrException(attr, c);
            }
        } else {
            assert(f->isMethod() && "Unknown feature kind");
            auto method = static_cast<Method *>(f);
            auto hidden =
                typeTable.getMethod(typeTable.getParentClass(c), f->getName());

            if (hidden) {
                // check that the signatures match
                if (method->getReturnType() != hidden->getReturnType()) {
                    throw InvalidMethodSignatureException(c, method);
                }

                auto methodB = method->begin(), methodE = method->end();
                auto hiddenB = hidden->begin(), hiddenE = hidden->end();

                for (; methodB != methodE && hiddenB != hiddenE; ++methodB, ++hiddenB) {
                    auto paramType = typeTable.getType((*methodB)->getType());
                    auto hiddenParamType = typeTable.getType((*hiddenB)->getType());

                    if (paramType != hiddenParamType) {
                        throw InvalidMethodSignatureException(c, method);
                    }
                }

                if (methodB != methodE || hiddenB != hiddenE) {
                    throw InvalidMethodSignatureException(c, method);
                }
            }
        }
    }
}

bool SemanticAnalysis::visit(Feature *f) { return ASTVisitor::visit(f); }

bool SemanticAnalysis::visit(Attribute *a) {
    auto attrType = typeTable.getType(a->getType());

    auto init = a->getInit();
    if (!init) {
        return true;
    }

    if (!visit(init)) {
        return false;
    }

    Type *expression_type = typeTable.getType(init);
    if (typeTable.getCommonType(attrType, expression_type) != attrType) {
        throw WrongTypeException(expression_type, attrType, init);
    }

    return true;
}

void SemanticAnalysis::checkMainClassAndMethod() {
    Class *main_class = typeTable.getType("Main")->getClass();
    Method *main_method = typeTable.getMethod(main_class, "main");

    if (!main_class || !main_method) {
        throw MethodNotFoundException("main", main_class);
    }
}

void SemanticAnalysis::checkInheritanceGraph() {
    for (auto inherit_class : *program) {
        Class *next_class = typeTable.getParentClass(inherit_class);
        while (next_class) {
            if (next_class == typeTable.getStringType()->getClass() ||
                next_class == inherit_class) {
                throw BadInheritanceException(inherit_class, next_class->getName());
            } else {
                next_class = typeTable.getParentClass(next_class);
            }
        }
    }
}

bool SemanticAnalysis::visit(Method *m) {
    std::unordered_set<std::string> paramNames;

    SymbolTable::Scope methodScope(symbolTable);
    for (auto param : *m) {

        int init_size = paramNames.size();
        paramNames.insert(param->getName());
        int after_size = paramNames.size();

        if (init_size == after_size) {
            throw DuplicateParamException(param, m);
        }

        if (!visit(param)) {
            return false;
        }
    }

    Type *method_type = typeTable.getType(m->getReturnType());
    auto body = m->getBody();
    if (body) {
        if (!visit(body)) {
            return false;
        }

        Type *body_type = typeTable.getType(body);

        if (typeTable.getCommonType(method_type, body_type) != method_type) {
            throw WrongTypeException(body_type, method_type, body);
        }
    } else {
        if (method_type != typeTable.getVoidType()) {
            throw WrongTypeException(typeTable.getVoidType(), method_type, nullptr);
        }
    }

    return true;
}

bool SemanticAnalysis::visit(FormalParam *param) {
    symbolTable.insert(param);
    typeTable.setType(param, typeTable.getType(param->getType()));
    return true;
}

bool SemanticAnalysis::visit(Expression *e) { return ASTVisitor::visit(e); }

bool SemanticAnalysis::visit(IntConstant *ic) {
    typeTable.setType(ic, typeTable.getIntType());
    return true;
}

bool SemanticAnalysis::visit(StringConstant *sc) {
    typeTable.setType(sc, typeTable.getStringType());
    return true;
}

bool SemanticAnalysis::visit(NullConstant *nc) {
    typeTable.setType(nc, typeTable.getNullType());
    return true;
}

bool SemanticAnalysis::visit(Symbol *s) {
    auto who = symbolTable.lookup(s->getName());
    typeTable.setType(s, typeTable.getType(who));

    return true;
}

bool SemanticAnalysis::visit(Block *b) {
    SymbolTable::Scope blockScope(symbolTable);

    // visit all expressions first
    if (!ASTVisitor::visit(b)) {
        return false;
    }

    // set the type to match the type of the last expression
    if (b->begin() != b->end()) {
        typeTable.setType(b, typeTable.getType(b->back()));
    } else {
        typeTable.setType(b, typeTable.getVoidType());
    }

    return true;
}

bool SemanticAnalysis::visit(Assignment *a) {
    auto rhs = a->getExpression();

    if (!rhs) {
        throw MissingOperandException(a);
    }

    if (!visit(rhs)) {
        return false;
    }

    auto symbol_type = typeTable.getType(symbolTable.lookup(a->getSymbol()));
    auto expression_type = typeTable.getType(rhs);
    if (typeTable.getCommonType(symbol_type, expression_type) != symbol_type) {
        throw IncompatibleOperandsException(rhs, symbol_type, expression_type);
    }

    typeTable.setType(a, symbol_type);

    return true;
}

bool SemanticAnalysis::visit(BinaryOperator *bo) {
    auto lhs = bo->getLHS();
    auto rhs = bo->getRHS();

    if (!lhs || !rhs) {
        throw MissingOperandException(bo);
    }

    assert(bo->getOperatorKind() != BinaryOperator::Invalid &&
           "Can't have invalid unary operators");

    if (!visit(lhs) || !visit(rhs))
    {
        return false;
    }

    if (bo->getOperatorKind() != BinaryOperator::Equal &&
            bo->getOperatorKind() != BinaryOperator::Add) {
        if (typeTable.getType(lhs) != typeTable.getIntType() ||
            typeTable.getType(rhs) != typeTable.getIntType()) {

            throw WrongTypeException(typeTable.getType(lhs), typeTable.getIntType(), bo);
        }
        typeTable.setType(bo, typeTable.getIntType());
    } else {
        if (bo->getOperatorKind() == BinaryOperator::Equal) {
            auto lhs_type = typeTable.getType(lhs);
            auto rhs_type = typeTable.getType(rhs);
            if (lhs_type == typeTable.getIntType() &&
                rhs_type == typeTable.getIntType()) {

                typeTable.setType(bo, typeTable.getIntType());
            } else {
                if (lhs_type == typeTable.getIntType() &&
                    rhs_type == typeTable.getStringType() ||
                    lhs_type == typeTable.getStringType() &&
                    rhs_type == typeTable.getIntType()) {

                    typeTable.setType(bo, typeTable.getIntType());
                } else {
                    if (lhs_type == typeTable.getStringType() &&
                        rhs_type == typeTable.getStringType()) {

                        typeTable.setType(bo, typeTable.getIntType());
                    } else {
                        if (lhs_type == typeTable.getNullType() &&
                            rhs_type == typeTable.getNullType()) {

                            typeTable.setType(bo, typeTable.getIntType());
                        } else {
                            auto common_type_lhs = typeTable.getCommonType(typeTable.getObjectType(), lhs_type);
                            auto common_type_rhs = typeTable.getCommonType(typeTable.getObjectType(), rhs_type);
                            if (common_type_lhs == typeTable.getObjectType() &&
                                rhs_type == typeTable.getIntType() ||
                                common_type_rhs == typeTable.getObjectType() &&
                                lhs_type == typeTable.getIntType()) {

                                throw WrongTypeException(typeTable.getType(lhs), typeTable.getIntType(), bo);
                            }
                            if (common_type_lhs == typeTable.getObjectType() &&
                                common_type_rhs == typeTable.getObjectType()) {

                                typeTable.setType(bo, typeTable.getIntType());
                            } else {
                                if (common_type_lhs == typeTable.getObjectType() &&
                                    rhs_type == typeTable.getNullType() ||
                                    common_type_rhs == typeTable.getObjectType() &&
                                    lhs_type == typeTable.getNullType()) {

                                    typeTable.setType(bo, typeTable.getIntType());
                                } else {
                                    throw WrongTypeException(typeTable.getType(lhs), typeTable.getIntType(), bo);
                                }
                            }
                        }
                    }
                }
            }
        }

        if (bo->getOperatorKind() == BinaryOperator::Add) {
            auto lhs_type = typeTable.getType(lhs);
            auto rhs_type = typeTable.getType(rhs);
            if (lhs_type == typeTable.getIntType() &&
                rhs_type == typeTable.getIntType()) {

                typeTable.setType(bo, typeTable.getIntType());
            } else {
                if (lhs_type == typeTable.getIntType() &&
                    rhs_type == typeTable.getStringType() ||
                    lhs_type == typeTable.getStringType() &&
                    rhs_type == typeTable.getIntType()) {

                    typeTable.setType(bo, typeTable.getStringType());
                } else {
                    if (lhs_type == typeTable.getStringType() &&
                        rhs_type == typeTable.getStringType()) {

                        typeTable.setType(bo, typeTable.getStringType());
                    } else {
                        throw WrongTypeException(typeTable.getType(lhs), typeTable.getIntType(), bo);
                    }
                }
            }
        }
    }

    return true;
}

bool SemanticAnalysis::visit(UnaryOperator *uo) {
    auto operand = uo->getOperand();

    if (!operand) {
        throw MissingOperandException(uo);
    }

    assert(uo->getOperatorKind() != UnaryOperator::Invalid &&
           "Can't have invalid unary operators");

    if (!visit(operand)) {
        return false;
    }

    if (typeTable.getType(operand) != typeTable.getIntType()) {
        throw WrongTypeException(typeTable.getType(operand), typeTable.getIntType(),
                                 uo);
    }

    typeTable.setType(uo, typeTable.getIntType());

    return true;
}

bool SemanticAnalysis::visit(Cast *c) {
    auto toCast = c->getExpressionToCast();

    if (!toCast) {
        throw MissingOperandException(c);
    }

    if (!visit(toCast)) {
        return false;
    }

    typeTable.setType(c, typeTable.getType(c->getType()));

    return true;
}

bool SemanticAnalysis::visit(Substring *s) {
    auto str = s->getString();
    auto start = s->getStart();
    auto end = s->getEnd();

    if (!str || !start || !end) {
        throw MissingOperandException(s);
    }

    if (!visit(str) || !visit(start) || !visit(end)) {
        return false;
    }

    if (typeTable.getType(str) != typeTable.getStringType()) {
        throw WrongTypeException(typeTable.getType(str), typeTable.getStringType(),
                                 s);
    }

    if (typeTable.getType(start) != typeTable.getIntType()) {
        throw WrongTypeException(typeTable.getType(start), typeTable.getIntType(),
                                 s);
    }

    if (typeTable.getType(end) != typeTable.getIntType()) {
        throw WrongTypeException(typeTable.getType(end), typeTable.getIntType(), s);
    }

    typeTable.setType(s, typeTable.getStringType());

    return true;
}

bool SemanticAnalysis::visit(Dispatch *d) {
    auto obj = d->getObject();

    if (obj) {
        if (!visit(obj)) {
            return false;
        }
    } else {
        auto self = symbolTable.lookup("self");
        obj = dynamic_cast<Expression *>(self);
        assert(obj && "Invalid lookup");
    }

    auto objType = typeTable.getType(obj);
    auto objClass = objType->getClass();
    if (!objClass) {
        throw DispatchOnInvalidObjException(d->getName(), objType);
    }

    auto method = typeTable.getMethod(objClass, d->getName());
    if (!method) {
        throw MethodNotFoundException(d->getName(), objClass);
    }

    auto cnt_method = std::distance(method->begin(), method->end());
    auto cnt_dispatch = std::distance(d->begin(), d->end());
    if (cnt_method < cnt_dispatch) {
        throw TooManyArgsException(method->getName(), d->getObject());
    }
    if (cnt_method > cnt_dispatch) {
        throw NotEnoughArgsException(method->getName(), d->getObject());
    }

    auto paramIt = method->begin();

    for (auto arg : *d) {
        if (!visit(arg)) {
            return false;
        }
        auto paramIt_type = typeTable.getType((*paramIt)->getType());
        auto arg_type = typeTable.getType(arg);
        paramIt++;

        if (paramIt_type == typeTable.getStringType() &&
            arg_type == typeTable.getIntType()) {

            continue;
        }
        if (arg_type != typeTable.getNullType()) {
            auto common_type = typeTable.getCommonType(paramIt_type, arg_type);

            if (common_type != paramIt_type) {
                throw WrongTypeException(arg_type, paramIt_type, d);
            }
        }
    }

    auto returnString = method->getReturnType();
    Type *returnType = typeTable.getType(returnString);

    typeTable.setType(d, returnType);

    return true;
}

bool SemanticAnalysis::visit(StaticDispatch *d) {
    auto obj = d->getObject();

    if (obj) {
        if (!visit(obj)) {
            return false;
        }
    } else {
        auto self = symbolTable.lookup("self");
        obj = dynamic_cast<Expression *>(self);
        assert(obj && "Invalid lookup");
    }

    auto objType = typeTable.getType(obj);
    auto staticObjectType = typeTable.getType(d->getType());
    if (typeTable.getCommonType(objType, staticObjectType) != staticObjectType) {
        throw DispatchOnInvalidObjException(d->getName(), staticObjectType);
    }
    auto objClass = staticObjectType->getClass();
    if (!objClass) {
        throw DispatchOnInvalidObjException(d->getName(), staticObjectType);
    }

    auto method = typeTable.getMethod(objClass, d->getName());
    if (!method) {
        throw MethodNotFoundException(d->getName(), objClass);
    }

    auto cnt_method = std::distance(method->begin(), method->end());
    auto cnt_dispatch = std::distance(d->begin(), d->end());
    if (cnt_method < cnt_dispatch) {
        throw TooManyArgsException(method->getName(), d->getObject());
    }
    if (cnt_method > cnt_dispatch) {
        throw NotEnoughArgsException(method->getName(), d->getObject());
    }

    auto paramIt = method->begin();
    for (auto arg : *d) {
        if (!visit(arg)) {
            return false;
        }

        auto paramIt_type = typeTable.getType((*paramIt)->getType());
        auto arg_type = typeTable.getType(arg);
        if (!(paramIt_type == typeTable.getStringType() && arg_type == typeTable.getIntType())) {
            if (typeTable.getCommonType(paramIt_type, arg_type) != paramIt_type) {
                throw WrongTypeException(arg_type, paramIt_type, d);
            }
        }

        paramIt = paramIt++;
    }

    auto returnString = method->getReturnType();
    Type *returnType = typeTable.getType(returnString);

    typeTable.setType(d, returnType);

    return true;
}

bool SemanticAnalysis::visit(NewObject *n) {
    auto type = typeTable.getType(n->getType());

    if (!type->getClass()) {
        throw WrongTypeException(type, n);
    }

    typeTable.setType(n, type);
    return true;
}

bool SemanticAnalysis::visit(IfStatement *i) {
    auto condExpr = i->getCond();
    auto thenExpr = i->getThen();
    auto elseExpr = i->getElse();

    if (!condExpr) {
        throw MissingIfCondException(i);
    }
    if (!visit(condExpr)) {
        return false;
    }
    if (typeTable.getType(condExpr) != typeTable.getIntType()) {
        throw WrongTypeException(typeTable.getType(condExpr), typeTable.getIntType(), condExpr);
    }
    if (!thenExpr) {
        throw MissingIfThenException(i);
    } else {
        SymbolTable::Scope ifScope(symbolTable);
        if (!visit(thenExpr)) {
            return false;
        }
    }
    if (elseExpr) {
        SymbolTable::Scope ifScope(symbolTable);
        if (!visit(elseExpr)) {
            return false;
        }
    }
    if (!elseExpr) {
        typeTable.setType(i, typeTable.getVoidType());
    } else {
        if (typeTable.getType(thenExpr) == typeTable.getVoidType() ||
            typeTable.getType(elseExpr) == typeTable.getVoidType()) {
            
            typeTable.setType(i, typeTable.getVoidType());
        } else {
            typeTable.setType(i, typeTable.getCommonType(typeTable.getType(thenExpr), typeTable.getType(elseExpr)));
        }
    }

    return true;
}

bool SemanticAnalysis::visit(WhileStatement *w) {
    auto cond = w->getCond();

    if (!cond) {
        throw MissingWhileCondException(w);
    }

    if (!visit(cond)) {
        return false;
    }

    if (typeTable.getType(cond) != typeTable.getIntType()) {
        throw WrongTypeException(typeTable.getType(cond), typeTable.getIntType(),
                                 cond);
    }

    auto body = w->getBody();
    if (body) {
        SymbolTable::Scope whileScope(symbolTable);
        if (!visit(body)) {
            return false;
        }
    }

    typeTable.setType(w, typeTable.getVoidType());

    return true;
}

bool SemanticAnalysis::visit(LocalDefinition *local) {
    symbolTable.insert(local);
    typeTable.setType(local, typeTable.getType(local->getType()));

    if (local->getInit()) {
        if (!visit(local->getInit())) {
            return false;
        }
    }

    return true;
}

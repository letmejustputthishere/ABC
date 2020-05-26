#include <vector>
#include "ast_opt/ast/ArithmeticExpr.h"
#include "ast_opt/ast/Variable.h"

ArithmeticExpr::ArithmeticExpr(AbstractExpr *left, ArithmeticOp op, AbstractExpr *right) {
  setAttributes(left, new Operator(op), right);
}

ArithmeticExpr::ArithmeticExpr(ArithmeticOp op) {
  setAttributes(nullptr, new Operator(op), nullptr);
}

ArithmeticExpr::ArithmeticExpr() {
  setAttributes(nullptr, nullptr, nullptr);
}

void ArithmeticExpr::accept(Visitor &v) {
  v.visit(*this);
}

std::string ArithmeticExpr::getNodeType() const {
  return "ArithmeticExpr";
}

ArithmeticExpr::~ArithmeticExpr() = default;

ArithmeticExpr *ArithmeticExpr::clone() const {
  return new ArithmeticExpr(this->getLeft()->clone(),
                            this->getOperator()->clone(),
                            this->getRight()->clone());
}
std::string ArithmeticExpr::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}

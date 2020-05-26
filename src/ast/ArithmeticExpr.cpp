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

void ArithmeticExpr::setLeft(AbstractExpr *newLeft) {
  //TODO: Remove old
  left = newLeft;
  if (newLeft) newLeft->setParent(this);
}

void ArithmeticExpr::setOperator(Operator *newOperator) {
  //TODO: Remove old
  op = newOperator;
  if (newOperator) newOperator->setParent(this);

}
void ArithmeticExpr::setRight(AbstractExpr *newRight) {
  //TODO: Remove old
  right = newRight;
  if (newRight) newRight->setParent(this);

}
std::vector<AbstractNode *> ArithmeticExpr::getChildren() {
  return {left, op, right};
}
std::vector<const AbstractNode *> ArithmeticExpr::getChildren() const {
  return {left, op, right};
}
void ArithmeticExpr::removeChildren() {
  setLeft(nullptr);
  setOperator(nullptr);
  setRight(nullptr);
}
AbstractExpr *ArithmeticExpr::getLeft() const {
  return left;
}
Operator *ArithmeticExpr::getOperator() const {
  return op;
}
AbstractExpr *ArithmeticExpr::getRight() const {
  return right;
}

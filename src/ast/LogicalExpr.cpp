#include "ast_opt/ast/LogicalExpr.h"
#include "ast_opt/ast/Variable.h"

void LogicalExpr::accept(Visitor &v) {
  v.visit(*this);
}

std::string LogicalExpr::getNodeType() const {
  return "LogicalExpr";
}

LogicalExpr::LogicalExpr() {
  setAttributes(nullptr, nullptr, nullptr);
}

LogicalExpr::LogicalExpr(LogCompOp op) {
  setAttributes(nullptr, new Operator(op), nullptr);
}

LogicalExpr *LogicalExpr::clone() const {
  return new LogicalExpr(getLeft()->clone(),
                         getOperator()->clone(),
                         getRight()->clone());
}

std::string LogicalExpr::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}
AbstractExpr *LogicalExpr::getLeft() const {
  return left;
}
void LogicalExpr::setLeft(AbstractExpr *newleft) {
  left = newleft;
  if (left) left->setParent(this);
}
void LogicalExpr::setOperator(Operator *newOperator) {
  op = newOperator;
  if (op) op->setParent(this);
}
void LogicalExpr::setRight(AbstractExpr *newRight) {
  right = newRight;
  if (right) right->setParent(this);
}
const std::vector<AbstractNode *> &LogicalExpr::getChildren() const {
  return {left, op, right};
}
void LogicalExpr::removeChildren() {
  setLeft(nullptr);
  setOperator(nullptr);
  setRight(nullptr);
}
Operator *LogicalExpr::getOperator() const {
  return op;
}
AbstractExpr *LogicalExpr::getRight() const {
  return right;
}

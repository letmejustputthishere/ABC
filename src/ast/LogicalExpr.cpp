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

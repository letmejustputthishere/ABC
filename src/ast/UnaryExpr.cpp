#include "ast_opt/ast/UnaryExpr.h"

json UnaryExpr::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["operator"] = getOperator() ? getOperator()->getOperatorString() : "";
  j["rightOperand"] = getRight() ? getRight()->toJson() : "";
  return j;
}

UnaryExpr::UnaryExpr(UnaryOp op, AbstractExpr *right) {
  setAttributes(op, right);
}

void UnaryExpr::accept(Visitor &v) {
  v.visit(*this);
}

Operator *UnaryExpr::getOperator() const {
  return dynamic_cast<Operator *>(children.at(0));
}

AbstractExpr *UnaryExpr::getRight() const {
  return dynamic_cast<AbstractExpr *>(children.at(1));
}

std::string UnaryExpr::getNodeType() const {
  return "UnaryExpr";
}

UnaryExpr::~UnaryExpr() {
  for (auto &child : getChildren()) delete child;
}

int UnaryExpr::getMaxNumberChildren() {
  return 2;
}

void UnaryExpr::setAttributes(UnaryOp op, AbstractExpr *expr) {
  removeChildren();
  auto nodesToBeAdded = std::vector<AbstractNode *>({new Operator(op), expr});
  children = nodesToBeAdded;
  for(auto &c: children) {
    c->setParent(this);
  }
}

UnaryExpr *UnaryExpr::clone() const {
  try {
    return new UnaryExpr(std::get<UnaryOp>(this->getOperator()->getOperatorSymbol()),
                         this->getRight()->clone());
  } catch (std::bad_variant_access &exc) {
    throw std::runtime_error(
        "Failed to clone UnaryExpr - unexpected Operator encountered! Expected operator of Enum UnaryOp.");
  }
}

bool UnaryExpr::isEqual(AbstractExpr *other) {
  if (auto otherAsUExp = dynamic_cast<UnaryExpr *>(other)) {
    auto sameOperator = this->getOperator()->equals(otherAsUExp->getOperator()->getOperatorSymbol());
    auto sameValue = this->getRight()->isEqual(otherAsUExp->getRight());
    return sameOperator && sameValue;
  }
  return false;
}
std::string UnaryExpr::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}
std::vector<std::string> UnaryExpr::getVariableIdentifiers() {
  return getRight()->getVariableIdentifiers();
}

std::vector<Variable *> UnaryExpr::getVariables() {
  return getRight()->getVariables();
}
void UnaryExpr::setOperator(Operator *newOperator) {
  //TODO remove old
  children[0] = newOperator;
  newOperator->setParent(this);
}

const std::vector<AbstractNode *> &UnaryExpr::getChildren() const {
 //TODO IMPLEMENT
 return {};
}
void UnaryExpr::removeChildren() {
//TODO IMPLEMENT
}

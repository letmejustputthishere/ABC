#include "ast_opt/ast/Transpose.h"

std::string Transpose::getNodeType() const {
  return std::string("Transpose");
}

void Transpose::accept(Visitor &v) {
  v.visit(*this);
}

Transpose::Transpose(AbstractExpression *operand) {
  removeChildren();
  children.push_back(operand);
  operand->setParent(this);
}

AbstractExpression *Transpose::getOperand() const {
  return dynamic_cast<AbstractExpression *>(children.at(0));
}

Transpose *Transpose::clone() const {
  return new Transpose(getOperand()->clone());
}

json Transpose::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["operand"] = getOperand()->toJson();
  return j;
}

std::vector<std::string> Transpose::getVariableIdentifiers() {
  return getOperand()->getVariableIdentifiers();
}

std::vector<Variable *> Transpose::getVariables() {
  return getOperand()->getVariables();
}

bool Transpose::contains(Variable *var) {
  return getOperand()->contains(var);
}

bool Transpose::isEqual(AbstractExpression *other) {
  return AbstractExpression::isEqual(other);
}

int Transpose::getMaxNumberChildren() {
  return 1;
}

std::string Transpose::toString(bool printChildren) const {
  return AbstractNode::toStringHelper(printChildren, {});
}

Transpose::Transpose() = default;
std::vector<AbstractNode *> Transpose::getChildren() {
  //TODO IMPLEMENT
  return {};
}
std::vector<const AbstractNode *> Transpose::getChildren() const {
  //TODO IMPLEMENT
  return {};
}
void Transpose::removeChildren() {
  //TODO IMPLEMENT
}

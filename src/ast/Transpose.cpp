#include "ast_opt/ast/Transpose.h"

std::string Transpose::getNodeType() const {
  return std::string("Transpose");
}

void Transpose::accept(Visitor &v) {
  v.visit(*this);
}

Transpose::Transpose(AbstractExpr *operand) {
  removeChildren();
  children.push_back(operand);
  operand->setParent(this);
}

AbstractExpr *Transpose::getOperand() const {
  return dynamic_cast<AbstractExpr *>(children.at(0));
}

Transpose *Transpose::clone(bool keepOriginalUniqueNodeId) const {
  auto clonedNode = new Transpose(getOperand()->clone(keepOriginalUniqueNodeId)->castTo<AbstractExpr>());
  clonedNode->updateClone(keepOriginalUniqueNodeId, this);
  return clonedNode;
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

bool Transpose::isEqual(AbstractExpr *other) {
  return AbstractExpr::isEqual(other);
}

int Transpose::getMaxNumberChildren() {
  return 1;
}

std::string Transpose::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}

AbstractNode *Transpose::cloneFlat() {
  return new Transpose();
}

Transpose::Transpose() = default;

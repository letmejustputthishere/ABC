#include "ast_opt/ast/GetMatrixSize.h"
#include "ast_opt/ast/AbstractExpr.h"
#include "ast_opt/ast/LiteralInt.h"

json GetMatrixSize::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["matrix"] = getMatrixOperand()->toJson();
  j["dimension"] = getDimensionParameter()->toJson();
  return j;
}

bool GetMatrixSize::isEqual(AbstractExpr *other) {
  if (auto otherGetMatrixSize = dynamic_cast<GetMatrixSize *>(other)) {
    return getMatrixOperand()->isEqual(otherGetMatrixSize->getMatrixOperand())
        && getDimensionParameter()->isEqual(otherGetMatrixSize->getDimensionParameter());
  }
  return false;
}

std::string GetMatrixSize::getNodeType() const {
  return std::string("GetMatrixSize");
}

int GetMatrixSize::getMaxNumberChildren() {
  return 2;
}

void GetMatrixSize::accept(Visitor &v) {
  v.visit(*this);
}

std::string GetMatrixSize::toString(bool printChildren) const {
  return AbstractNode::toStringHelper(printChildren, {});
}

GetMatrixSize *GetMatrixSize::clone() const {
  return new GetMatrixSize(getMatrixOperand()->clone(), getDimensionParameter()->clone());
}

GetMatrixSize::GetMatrixSize(AbstractExpr *matrix, AbstractExpr *requestedDimension) {
  removeChildren();
  children = {matrix, requestedDimension};
  for (auto &c: children) {
    c->setParent(this);
  }
}
std::vector<std::string> GetMatrixSize::getVariableIdentifiers() {
  auto results = getMatrixOperand()->getVariableIdentifiers();
  auto vec = getDimensionParameter()->getVariableIdentifiers();
  if (!vec.empty()) {
    results.insert(results.end(), vec.begin(), vec.end());
  }
  return results;
}

std::vector<Variable *> GetMatrixSize::getVariables() {
  auto results = getMatrixOperand()->getVariables();
  auto vec = getDimensionParameter()->getVariables();
  if (!vec.empty()) {
    results.insert(results.end(), vec.begin(), vec.end());
  }
  return results;
}

AbstractExpr *GetMatrixSize::getMatrixOperand() const {
  return dynamic_cast<AbstractExpr *>(children.at(0));
}

AbstractExpr *GetMatrixSize::getDimensionParameter() const {
  return dynamic_cast<AbstractExpr *>(children.at(1));
}

GetMatrixSize::~GetMatrixSize() = default;

std::vector<AbstractNode *> GetMatrixSize::getChildren() {
  //TODO IMPLEMENT
  return {};
}
std::vector<const AbstractNode *> GetMatrixSize::getChildren() const {
  //TODO IMPLEMENT
  return {};
}
void GetMatrixSize::removeChildren() {
  //TODO IMPLEMENT
}

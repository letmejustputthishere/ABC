#include "ast_opt/ast/MatrixAssignm.h"
#include "ast_opt/ast/MatrixElementRef.h"
#include "ast_opt/ast/ArithmeticExpr.h"

MatrixAssignm::~MatrixAssignm() = default;

MatrixAssignm::MatrixAssignm(MatrixElementRef *assignmentTarget, AbstractExpression *value) {
  setAttributes(dynamic_cast<AbstractExpression *>(assignmentTarget), value);
}

void MatrixAssignm::setAttributes(AbstractExpression *assignmTarget, AbstractExpression *value) {
  removeChildren();
  children = {assignmTarget, value};
  for(auto &c: children) {
    c->setParent(this);
  }
}

std::string MatrixAssignm::getNodeType() const {
  return std::string("MatrixAssignm");
}

int MatrixAssignm::getMaxNumberChildren() {
  return 2;
}

void MatrixAssignm::accept(Visitor &v) {
  v.visit(*this);
}

std::string MatrixAssignm::toString(bool printChildren) const {
  return AbstractNode::toStringHelper(printChildren, {});
}

MatrixElementRef *MatrixAssignm::getAssignmTarget() const {
  return dynamic_cast<MatrixElementRef *>(children.at(0));
}

std::string MatrixAssignm::getAssignmTargetString() const {
  std::stringstream ss;
  // Build a string in the format <matrixIdentifier>[<rowIndexExpr>][<columnIndexExpr>].
  // Unfortunately, toString also includes a node's name such that we get a string like
  // "Variable (M)[LiteralInt (32)][LiteralInt (1)]" where the compacter "M[32][1]" would be desired instead.
  auto assignmTarget = getAssignmTarget();
  ss << assignmTarget->getOperand()->toString(true);
  ss << "[" << assignmTarget->getRowIndex()->toString(true) << "]";
  if (assignmTarget->getColumnIndex()!=nullptr) {
    ss << "[" << assignmTarget->getColumnIndex()->toString(true) << "]";
  }
  std::string res(ss.str());
  // remove any new line characters
  res.erase(std::remove_if(res.begin(), res.end(), [](char c) { return c=='\n'; }), res.end());
  // replace tabs by whitespace characters
  std::replace(res.begin(), res.end(), '\t', ' ');
  return res;
}

AbstractExpression *MatrixAssignm::getValue() const {
  return dynamic_cast<AbstractExpression *>(children.at(1));
}

MatrixAssignm *MatrixAssignm::clone() const {
  return new MatrixAssignm(getAssignmTarget()->clone(), getValue()->clone());
}

json MatrixAssignm::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["target"] = getAssignmTarget()->toJson();
  j["value"] = getValue() ? getValue()->toJson() : "";
  return j;
}

AbstractBinaryExpr *MatrixAssignm::contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree) {
  return getValue()->contains(aexpTemplate, excludedSubtree);
}

bool MatrixAssignm::isEqual(const AbstractNode *as) const {
  if (auto otherMxAssignm = dynamic_cast<const MatrixAssignm *>(as)) {
    return getAssignmTarget()->isEqual(otherMxAssignm->getAssignmTarget())
        && getValue()->isEqual(otherMxAssignm->getValue());
  }
  return false;
}

std::vector<AbstractNode *> MatrixAssignm::getChildren()  {
  //TODO IMPLEMENT
  return {};
}
std::vector<const AbstractNode *> MatrixAssignm::getChildren() const {
  //TODO IMPLEMENT
  return {};
}
void MatrixAssignm::removeChildren() {
  //TODO IMPLEMENT
}

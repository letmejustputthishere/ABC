#include "ast_opt/ast/Return.h"

#include <utility>

json Return::toJson() const {
  json j;
  j["type"] = getNodeType();
  // build the string of result values
  json array = json::array();
  for (auto &expr : getReturnExpressions()) array.push_back(expr->toJson());
  j["values"] = array;
  return j;
}

Return::Return(AbstractExpr *returnValue) {
  setAttributes({returnValue});
}

Return::Return(std::vector<AbstractExpr *> returnValues) {
  setAttributes(std::move(returnValues));
}

void Return::accept(Visitor &v) {
  v.visit(*this);
}

std::string Return::getNodeType() const {
  return "Return";
}

Return::~Return() {
  for (auto &child : getChildren()) delete child;
}

Return::Return() = default;

int Return::getMaxNumberChildren() {
  return -1;
}

void Return::setAttributes(std::vector<AbstractExpr *> returnExpr) {
  removeChildren();
  // we need to convert vector of AbstractExpr* into vector of AbstractNode* prior calling addChildren
  returnExpressions = returnExpr;
  for (auto &c: returnExpr) {
    c->setParent(this);
  }
}

std::vector<AbstractExpr *> Return::getReturnExpressions() const {
  return returnExpressions;
}

Return *Return::clone() const {
  std::vector<AbstractExpr *> returnValues;
  for (auto &child : getReturnExpressions())
    returnValues.push_back(child->clone());
  return new Return(returnValues);
}

std::string Return::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}

bool Return::isEqual(const AbstractNode *as) const {
  if (auto asAsReturn = dynamic_cast<const Return *>(as)) {
    auto thisRetExp = this->getReturnExpressions();
    auto otherRetExp = asAsReturn->getReturnExpressions();
    // check equality of every returned value
    bool sameReturnValues = std::equal(thisRetExp.begin(), thisRetExp.end(),
                                       otherRetExp.begin(), otherRetExp.end(),
                                       [](AbstractExpr *first, AbstractExpr *second) {
                                         return first->isEqual(second);
                                       });
    return sameReturnValues;
  }
  return false;
}
void Return::addReturnExpr(AbstractExpr *returnExpr) {
  children.push_back(returnExpr);
  returnExpr->setParent(this);
}

std::vector<AbstractNode *> Return::getChildren() {
  return std::vector<AbstractNode *>(returnExpressions.begin(), returnExpressions.end());
}
std::vector<const AbstractNode *> Return::getChildren() const {
  return std::vector<const AbstractNode *>(returnExpressions.begin(), returnExpressions.end());
}
void Return::removeChildren() {
//TODO ACTUAL REMOVE
  returnExpressions.clear();
}

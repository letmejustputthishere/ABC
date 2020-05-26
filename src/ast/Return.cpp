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
  std::vector<AbstractNode *> returnExprAsNodes(returnExpr.begin(), returnExpr.end());
  children = returnExprAsNodes;
  for(auto &c: children) {
    c->setParent(this);
  }
}

std::vector<AbstractExpr *> Return::getReturnExpressions() const {
  std::vector<AbstractExpr *> vec;
  for (auto &child : getChildrenNonNull()) vec.push_back(child->castTo<AbstractExpr>());
  return vec;
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

bool Return::isEqual(AbstractStatement *as) {
  if (auto asAsReturn = dynamic_cast<Return *>(as)) {
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

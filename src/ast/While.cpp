#include <exception>
#include "ast_opt/ast/While.h"
#include "ast_opt/ast/LiteralBool.h"

While::While(AbstractExpr *condition, AbstractStatement *body) {
  setAttributes(condition, body);
}

json While::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["condition"] = getCondition()->toJson();
  j["body"] = getBody()->toJson();
  return j;
}

void While::accept(Visitor &v) {
  v.visit(*this);
}

AbstractExpr *While::getCondition() const {
  return dynamic_cast<AbstractExpr *>(children.at(0));
}

AbstractStatement *While::getBody() const {
  return dynamic_cast<AbstractStatement *>(children.at(1));
}

std::string While::getNodeType() const {
  return "While";
}

While *While::clone(bool keepOriginalUniqueNodeId) const {
  auto clonedWhile = new While(getCondition()->clone(keepOriginalUniqueNodeId)->castTo<AbstractExpr>(),
                               getBody()->clone(false)->castTo<AbstractStatement>());
  clonedWhile->updateClone(keepOriginalUniqueNodeId, this);
  return clonedWhile;
}

void While::setAttributes(AbstractExpr *loopCondition, AbstractStatement *loopBody) {
  removeChildren();
  addChildren({loopCondition, loopBody});
}

int While::getMaxNumberChildren() {
  return 2;
}


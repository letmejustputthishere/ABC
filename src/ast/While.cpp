#include <exception>
#include "ast_opt/ast/While.h"
#include "ast_opt/ast/LiteralBool.h"

While::While(AbstractExpression *condition, AbstractStatement *body) {
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

AbstractExpression *While::getCondition() const {
  return dynamic_cast<AbstractExpression *>(children.at(0));
}

AbstractStatement *While::getBody() const {
  return dynamic_cast<AbstractStatement *>(children.at(1));
}

std::string While::getNodeType() const {
  return "While";
}

While *While::clone() const {
  return new While(getCondition()->clone(), getBody()->clone());
}

void While::setAttributes(AbstractExpression *loopCondition, AbstractStatement *loopBody) {
  removeChildren();
  children = {loopCondition, loopBody};
  for (auto &c: children) {
    c->setParent(this);
  }
}

int While::getMaxNumberChildren() {
  return 2;
}

std::vector<AbstractNode *> While::getChildren() {
  //TODO IMPLEMENT
  return {};
}
std::vector<const AbstractNode *> While::getChildren() const {
  //TODO IMPLEMENT
  return {};
}
void While::removeChildren() {
  //TODO IMPLEMENT
}

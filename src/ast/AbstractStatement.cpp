#include <iostream>
#include "ast_opt/ast/AbstractStatement.h"
#include "ast_opt/ast/Block.h"
#include "ast_opt/ast/Call.h"
#include "ast_opt/ast/ArithmeticExpr.h"

json AbstractStatement::toJson() const {
  return json({"type", "AbstractStatement"});
}

AbstractBinaryExpr *AbstractStatement::contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree) {
  return nullptr;
}

AbstractNode::iterator AbstractStatement::begin() {
  //TODO
  return AbstractNode::iterator();
}
AbstractNode::const_iterator AbstractStatement::begin() const {
  //TODO
  return AbstractNode::const_iterator();
}
AbstractNode::iterator AbstractStatement::end() {
  //TODO
  return AbstractNode::iterator();
}
AbstractNode::const_iterator AbstractStatement::end() const {
  //TODO
  return AbstractNode::const_iterator();
}

std::ostream &operator<<(std::ostream &outs, const AbstractStatement &obj) {
  return outs << obj.toString(false);
}

void to_json(json &j, const AbstractStatement &absStat) {
  j = absStat.toJson();
}

void to_json(json &j, const AbstractStatement *absStat) {
  j = absStat->toJson();
}



#include "ast_opt/ast/If.h"
#include "ast_opt/ast/LiteralBool.h"
#include "ast_opt/ast/Block.h"

If::~If() = default;

If::If(std::unique_ptr<AbstractExpr> &&condition, std::unique_ptr<AbstractStatement> &&thenBranch, std::unique_ptr<
    AbstractStatement> &&elseBranch) {

}

If *If::clone() const {
  return new If(getCondition()->clone(),
                getThenBranch()->clone(),
                getElseBranch()->clone());
}

const AbstractExpr * If::getCondition() const {
  return children.at(0)->castTo<AbstractExpr>();
}

const Block * If::getThenBranch() const {
  return children.at(1) ? children.at(1)->castTo<Block>() : nullptr;
}

const Block * If::getElseBranch() const {
  return children.at(2) ? children.at(2)->castTo<Block>() : nullptr;
}

void If::setCondition(std::unique_ptr<AbstractExpr> &&newCondition) {
  newCondition->setParent(this);
  condition = std::move(newCondition);
}

void If::setThenBranch(std::unique_ptr<Block> &&newThenBranch) {
  newThenBranch->setParent(this);
  thenBranch = std::move(newThenBranch);
}

void If::setElseBranch(std::unique_ptr<Block> &&newElseBranch) {
  newElseBranch->setParent(this);
  elseBranch = std::move(newElseBranch);
}

int If::getMaxNumberChildren() {
  return 3;
}

std::vector<AbstractNode *> If::getChildren() {
  return {};
}
std::vector<const AbstractNode *> If::getChildren() const {
  return {};
}

void If::removeChildren() {
  //TODO: Remove old
  setCondition(nullptr);
  setThenBranch(nullptr);
  setElseBranch(nullptr);
}

AbstractNode::iterator If::begin() {
  return AbstractNode::iterator(std::make_unique<IfIteratorImpl<AbstractNode>>(*this, 0));
}

AbstractNode::const_iterator If::begin() const {
  return AbstractNode::const_iterator(std::make_unique<IfIteratorImpl<const AbstractNode>>(*this, 0));
}

AbstractNode::iterator If::end() {
  return AbstractNode::iterator(std::make_unique<IfIteratorImpl<AbstractNode>>(*this, 3));
}

AbstractNode::const_iterator If::end() const {
  return AbstractNode::const_iterator(std::make_unique<IfIteratorImpl<const AbstractNode>>(*this, 3));
}

std::string If::getNodeType() const {
  return "If";
}

json If::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["condition"] = getCondition()->toJson();
  if (getThenBranch()!=nullptr) j["thenBranch"] = getThenBranch()->toJson();
  if (getElseBranch()!=nullptr) j["elseBranch"] = getElseBranch()->toJson();
  return j;
}

void If::accept(Visitor &v) {
  v.visit(*this);
}
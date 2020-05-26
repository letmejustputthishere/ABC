#include "ast_opt/ast/If.h"
#include "ast_opt/ast/LiteralBool.h"
#include "ast_opt/ast/Block.h"

json If::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["condition"] = getCondition()->toJson();
  if (getThenBranch()!=nullptr) j["thenBranch"] = getThenBranch()->toJson();
  if (getElseBranch()!=nullptr) j["elseBranch"] = getElseBranch()->toJson();
  return j;
}

If::If(AbstractExpr *condition, AbstractStatement *thenBranch, AbstractStatement *elseBranch) {
  setAttributes(condition, thenBranch, elseBranch);
}

If::If(AbstractExpr *condition, AbstractStatement *thenBranch) {
  setAttributes(condition, thenBranch, nullptr);
}

void If::accept(Visitor &v) {
  v.visit(*this);
}

std::string If::getNodeType() const {
  return "If";
}

AbstractExpr *If::getCondition() const {
  return children.at(0)->castTo<AbstractExpr>();
}

Block *If::getThenBranch() const {
  return children.at(1) ? children.at(1)->castTo<Block>() : nullptr;
}

Block *If::getElseBranch() const {
  return children.at(2) ? children.at(2)->castTo<Block>() : nullptr;
}

If::~If() = default;

If *If::clone(bool keepOriginalUniqueNodeId) const {
  auto clonedNode = new If(getCondition()->clone(keepOriginalUniqueNodeId)->castTo<AbstractExpr>(),
                           getThenBranch()->clone(keepOriginalUniqueNodeId)->castTo<AbstractStatement>(),
                           getElseBranch()->clone(keepOriginalUniqueNodeId)->castTo<AbstractStatement>());
  clonedNode->updateClone(keepOriginalUniqueNodeId, this);
  return clonedNode;
}
int If::getMaxNumberChildren() {
  return 3;
}

void If::setAttributes(AbstractExpr *condition, AbstractStatement *thenBranch, AbstractStatement *elseBranch) {
  // update tree structure
  removeChildren();

  children.push_back(condition);

  auto thenBranchAsBlock = dynamic_cast<Block *>(thenBranch);
  if (!thenBranchAsBlock) {
    thenBranchAsBlock = new Block;
    thenBranchAsBlock->addStatement(thenBranch);
  }
  children.push_back(thenBranchAsBlock);
  thenBranchAsBlock->setParent(this);

  auto elseBranchAsBlock = dynamic_cast<Block *>(elseBranch);
  if (!elseBranchAsBlock) {
    elseBranchAsBlock = new Block;
    elseBranchAsBlock->addStatement(elseBranch);
  }
  children.push_back(elseBranchAsBlock);
  elseBranchAsBlock->setParent(this);

}
std::string If::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}

void If::setCondition(AbstractExpr *newCondition) {
  //TODO: Delete old
  newCondition->setParent(this);
  children[0] = newCondition;
}

void If::setThenBranch(Block *newThenBranch) {
  //TODO: Delete old
  newThenBranch->setParent(this);
  children[1] = newThenBranch;
}

void If::setElseBranch(Block *newElseBranch) {
  //TODO: Delete old
  newElseBranch->setParent(this);
  children[2] = newElseBranch;

}
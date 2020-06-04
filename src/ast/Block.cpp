#include <iostream>
#include <exception>
#include "ast_opt/ast/AbstractNode.h"
#include "ast_opt/ast/Block.h"
#include "ast_opt/ast/VarDecl.h"

json Block::toJson() const {
  json j = {{"type", getNodeType()},
            {"statements", getStatements()}};
  return j;
}

Block::Block(AbstractStatement *stat) {
  this->addStatement(stat);
}

Block::Block(std::vector<AbstractStatement *> statements) {
  if (statements.empty()) {
    throw std::logic_error("Block statement vector is empty!"
                           "If this is intended, use the parameter-less constructor instead.");
  }
  children = std::vector<AbstractNode *>(statements.begin(), statements.end());
  for (auto &c: children) {
    c->setParent(this);
  }
}

void Block::accept(Visitor &v) {
  v.visit(*this);
}

std::string Block::getNodeType() const {
  return "Block";
}

std::vector<AbstractStatement *> Block::getStatements() {
  std::vector<AbstractStatement *> stmts;
  stmts.reserve(countChildrenNonNull());
  for (auto c : getChildren()) {
    stmts.emplace_back(dynamic_cast<AbstractStatement *>(c));
  }
  return stmts;
}

std::vector<const AbstractStatement *> Block::getStatements() const {
  std::vector<const AbstractStatement *> stmts;
  stmts.reserve(countChildrenNonNull());
  for (auto c : getChildren()) {
    stmts.emplace_back(dynamic_cast<const AbstractStatement *>(c));
  }
  return stmts;
}

Block *Block::clone() const {
  std::vector<AbstractStatement *> clonedStatements;
  for (auto &statement : this->getStatements()) {
    clonedStatements.push_back(statement->clone());
  }
  return clonedStatements.empty() ? new Block() : new Block(clonedStatements);
}

int Block::getMaxNumberChildren() {
  return -1;
}

std::string Block::toString(bool printChildren) const {
  return AbstractNode::toStringHelper(printChildren, {});
}

bool Block::isEqual(const AbstractNode *otherBlockStatement) const {
  if (auto otherAsBlock = dynamic_cast<const Block *>(otherBlockStatement)) {
    auto thisStatements = getStatements();
    auto otherStatements = otherAsBlock->getStatements();

    auto thisStatIt = thisStatements.begin();
    auto otherStatIt = otherStatements.begin();

    // compare statement-per-statement
    for (; thisStatIt!=thisStatements.end() && otherStatIt!=otherStatements.end(); ++thisStatIt) {
      // break up if any pair of two compared statements do not match
      if (!(*thisStatIt)->isEqual(*otherStatIt)) return false;
      otherStatIt++;
    }

    // make sure that both blocks reached their end (otherwise one could have more than the other one but still be
    // recognized as being equal)
    return (thisStatIt==thisStatements.end()) && (otherStatIt==otherStatements.end());
  }
  return false;
}
void Block::addStatement(AbstractStatement *stmt) {
  children.push_back(stmt);
  if (stmt) {
    stmt->setParent(this);
  }
}
std::vector<AbstractNode *> Block::getChildren() {
  return children;
}

std::vector<const AbstractNode *> Block::getChildren() const {
  return std::vector<const AbstractNode*>(children.begin(),children.end());
}

void Block::removeChildren() {
  //TODO: Actually delete
  children.clear();
}

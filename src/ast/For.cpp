#include "ast_opt/ast/For.h"
#include "ast_opt/ast/AbstractExpr.h"
#include "ast_opt/ast/Block.h"

std::string For::getNodeType() const {
  return "For";
}

void For::accept(Visitor &v) {
  v.visit(*this);
}

For::For(AbstractStatement *initializer,
         AbstractExpr *condition,
         AbstractStatement *update,
         AbstractStatement *statementToBeExecuted) {
  setAttributes(initializer, condition, update, statementToBeExecuted);
}

Block *For::getInitializer() const {
  return dynamic_cast<Block *>(children.at(0));
}

AbstractExpr *For::getCondition() const {
  return dynamic_cast<AbstractExpr *>(children.at(1));
}

Block *For::getUpdate() const {
  return dynamic_cast<Block *>(children.at(2));
}

Block *For::getBody() const {
  return dynamic_cast<Block *>(children.at(3));
}

void For::setAttributes(AbstractStatement *initializer,
                        AbstractExpr *condition,
                        AbstractStatement *update,
                        AbstractStatement *body) {
  removeChildren();
  if (dynamic_cast<Block *>(initializer)==nullptr) {
    initializer = new Block(initializer);
  }
  if (dynamic_cast<Block *>(update)==nullptr) {
    update = new Block(update);
  }
  if (dynamic_cast<Block *>(body)==nullptr) {
    body = new Block(body);
  }
  addChildren({initializer, condition, update, body});
}

int For::getMaxNumberChildren() {
  return 4;
}

For *For::clone() const {

  auto clonedInitializer = (getInitializer()==nullptr)
                           ? nullptr
                           : getInitializer()->clone();
  auto clonedCondition = (getCondition()==nullptr)
                         ? nullptr
                         : getCondition()->clone();
  auto clonedUpdater = (getUpdate()==nullptr)
                       ? nullptr
                       : getUpdate()->clone();
  auto clonedBody = (getBody()==nullptr)
                    ? nullptr
                    : getBody()->clone();

  return new For(clonedInitializer, clonedCondition, clonedUpdater, clonedBody);
}

std::string For::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}

json For::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["condition"] = getCondition()->toJson();
  j["initializer"] = getInitializer()->toJson();
  j["update"] = getUpdate()->toJson();
  j["statement"] = getBody()->toJson();
  return j;
}

bool For::isEqual(AbstractStatement *other) {
  if (auto otherFor = dynamic_cast<For *>(other)) {
    auto sameInitializer = (getInitializer()==nullptr && otherFor->getInitializer()==nullptr)
        || (getInitializer()!=nullptr && otherFor->getInitializer()!=nullptr
            && getInitializer()->isEqual(otherFor->getInitializer()));
    auto sameCondition = (getCondition()==nullptr && otherFor->getCondition()==nullptr)
        || (getCondition()!=nullptr && otherFor->getCondition()!=nullptr
            && getCondition()->isEqual(otherFor->getCondition()));
    auto sameUpdateStmt = (getUpdate()==nullptr && otherFor->getUpdate()==nullptr)
        || (getUpdate()!=nullptr && otherFor->getUpdate()!=nullptr
            && getUpdate()->isEqual(otherFor->getUpdate()));
    auto sameBody = getBody()->isEqual(otherFor->getBody());
    return sameInitializer && sameCondition && sameUpdateStmt && sameBody;
  }
  return false;
}
void For::setInitializer(Block *initializer) {
  if (getInitializer()) {
    throw std::runtime_error("Cannot overwrite initializer.");
    // Because we cannot really "delete" nodes safely unless we're in a visitor?
  } else {
    children[0] = initializer;
    initializer->setParent(this);
  }
}

void For::setBody(Block *body) {
  if (getBody()) {
    throw std::runtime_error("Cannot overwrite body.");
    // Because we cannot really "delete" nodes safely unless we're in a visitor?
  } else {
    children[3] = body;
    body->setParent(this);
  }
}

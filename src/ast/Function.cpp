#include <utility>
#include <iostream>
#include "ast_opt/ast/Function.h"
#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/ArithmeticExpr.h"
#include "ast_opt/ast/Block.h"
#include "ast_opt/ast/Return.h"
#include "ast_opt/ast/AbstractStatement.h"

void Function::addParameter(FunctionParameter *param) {
  if (children.empty()) {
    auto pl = new ParameterList({param});
    children.push_back(pl);
    pl->setParent(this);
  }
  getParameterList()->addParameter(param);

}

Function::Function(std::string name, Block *pt) : name(std::move(name)) {
  children = {new ParameterList(), pt};
  for(auto &c: children) {
    c->setParent(this);
  }
}

Function::Function(std::string name) : name(std::move(name)) {
  children = {new ParameterList(), new Block()};
  for(auto &c: children) {
    c->setParent(this);
  }
}

Function::Function(std::string functionName, ParameterList *functionParameters,
                   Block *functionStatements) : name(std::move(functionName)) {
  children ={functionParameters, functionStatements};
  for(auto &c: children) {
    c->setParent(this);
  }
}

void Function::addStatement(AbstractStatement *statement) {
  if (children.empty()) {
    auto pl = new ParameterList();
    children.push_back(pl);
    pl->setParent(this);
  }
  if (children.size()==1) {// only Parameters
    // Introduce the wrapper Block
    auto b = new Block();
    children.push_back(b);
    b->setParent(this);
  }
  getBody()->addStatement(statement);
}

const std::string &Function::getName() const {
  return name;
}

void Function::accept(Visitor &v) {
  v.visit(*this);
}

std::vector<AbstractStatement *> Function::getBodyStatements() const {
  return getBody()->getStatements();
}

void to_json(json &j, const Function &func) {
  j = {
      {"type", func.getNodeType()},
      {"name", func.getName()},
      {"params", func.getParameters()},
      {"body", func.getBodyStatements()}};
}

json Function::toJson() const {
  json j = {
      {"type", getNodeType()},
      {"name", getName()},
      {"params", getParameters()},
      {"body", getBodyStatements()}
  };
  return j;
}

std::string Function::getNodeType() const {
  return "Function";
}

void Function::setParameterList(ParameterList *paramsVec) {
  if(children.empty()) {
    children.push_back(paramsVec);
  } else if(children[0]) {
    //TODO: Deletion ~children[0]
  } else {
    // children[0] exists but is a nullptr
    children[0] = paramsVec;
  }
  paramsVec->setParent(this);
}

Function *Function::clone() const {
  auto clonedParams = getParameterList() ? getParameterList()->clone() : nullptr;
  auto clonedBody = getBody() ? getBody()->clone() : nullptr;
  auto clonedNode = new Function(this->getName(), clonedParams, clonedBody);
  return clonedNode;
}

std::vector<FunctionParameter *> Function::getParameters() const {
  return children.empty() ? std::vector<FunctionParameter *>()
                          : dynamic_cast<ParameterList *>(children.at(0))->getParameters();
}

ParameterList *Function::getParameterList() const {
  return dynamic_cast<ParameterList *>(children.at(0));
}

Block *Function::getBody() const {
  return dynamic_cast<Block *>(children.at(1));
}

Function::Function(std::string functionName,
                   std::vector<FunctionParameter *> functionParameters,
                   std::vector<AbstractStatement *> functionStatements) : name(std::move(functionName)) {
  children = {new ParameterList(std::move(functionParameters)),
               new Block(std::move(functionStatements))};
  for(auto &c: children) {
    c->setParent(this);
  }
}

int Function::getMaxNumberChildren() {
  return 2;
}

std::string Function::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {getName()});
}

std::vector<AbstractNode *> Function::getChildren()  {
  //TODO IMPLEMENT
  return {};
}

void Function::removeChildren() {
  //TODO IMPLEMENT
}

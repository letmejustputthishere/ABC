#include "ast_opt/ast/ParameterList.h"

#include <utility>
#include "ast_opt/ast/FunctionParameter.h"

std::string ParameterList::getNodeType() const {
  return "ParameterList";
}

void ParameterList::accept(Visitor &v) {
  v.visit(*this);
}

ParameterList *ParameterList::clone() const {
  std::vector<FunctionParameter*> childrenCopy;
  childrenCopy.reserve(children.size());
  for (auto &c: children) {
    childrenCopy.push_back(c->clone()->castTo<FunctionParameter>());
  }
  return new ParameterList(childrenCopy);
}

ParameterList::ParameterList(std::vector<FunctionParameter *> parameters) : parameters(std::move(parameters)) {};

std::vector<FunctionParameter *> ParameterList::getParameters() {
    return parameters;
}

int ParameterList::getMaxNumberChildren() {
  return -1;
}

std::string ParameterList::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}
void ParameterList::addParameter(FunctionParameter *param) {
  parameters.push_back(param);
  param->setParent(this);
}
const std::vector<AbstractNode *> &ParameterList::getChildren() const {
  std::vector<AbstractNode*> v;
  v.reserve(parameters.size());
  for(auto &p : parameters) {
    v.push_back(p);
  }
  return v;
}
void ParameterList::removeChildren() {
//TODO removal
}

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
  return AbstractNode::toStringHelper(printChildren, {});
}
void ParameterList::addParameter(FunctionParameter *param) {
  parameters.push_back(param);
  param->setParent(this);
}
std::vector<AbstractNode *> ParameterList::getChildren()  {
  return std::vector<AbstractNode*>(parameters.begin(),parameters.end());
}
std::vector<const AbstractNode *> ParameterList::getChildren() const {
  return std::vector<const AbstractNode*>(parameters.begin(),parameters.end());
}
void ParameterList::removeChildren() {
//TODO removal
}

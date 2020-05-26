#include "ast_opt/ast/ParameterList.h"
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

ParameterList::ParameterList(std::vector<FunctionParameter *> parameters) {
  for (auto &fp : parameters) {
    addParameter(fp);
  }
}

std::vector<FunctionParameter *> ParameterList::getParameters() {
  std::vector<FunctionParameter *> params;
  params.reserve(children.size());
  for (auto &n : children) {
    params.emplace_back(dynamic_cast<FunctionParameter *>(n));
  }
  return params;
}

int ParameterList::getMaxNumberChildren() {
  return -1;
}

std::string ParameterList::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}
void ParameterList::addParameter(FunctionParameter *param) {
  children.push_back(param);
  param->setParent(this);
}

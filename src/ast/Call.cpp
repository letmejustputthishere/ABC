#include <typeindex>
#include <sstream>
#include <utility>
#include "ast_opt/ast/Call.h"
#include "ast_opt/ast/Function.h"
#include "ast_opt/ast/AbstractLiteral.h"
#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/LiteralBool.h"
#include "ast_opt/ast/LiteralString.h"

json Call::toJson() const {
  json j = {{"type", getNodeType()},
            {"arguments", getArguments()},
            {"function", getFunc()->toJson()}};
  return j;
}

void Call::accept(Visitor &v) {
  v.visit(*this);
}

std::string Call::getNodeType() const {
  return "Call";
}

Call::Call(Function *func) {
  setAttributes({}, func);
}

Call::Call(std::vector<FunctionParameter *> parameterValuesForCalledFunction, Function *func) {
  //TODO: Should no longer use FunctionParameter for arguments instead list of AbstractExpr?
  // How to actually map arguments to function parameters? Probably better to use a specifc FunctionArgument
  // that includes both the name of the parameter that it replaces and an AbstractExpr* value
  setAttributes(std::move(parameterValuesForCalledFunction), func);
}

Function *Call::getFunc() const {
  return dynamic_cast<Function *>(getChildAtIndex(1));
}

std::vector<FunctionParameter *> Call::getArguments() const {
  return dynamic_cast<ParameterList *>(getChildAtIndex(0))->getParameters();
}

AbstractExpr *Call::clone(bool keepOriginalUniqueNodeId) const{
  std::vector<FunctionParameter *> clonedArgs;
  for (auto &arg : getArguments()) {
    clonedArgs.push_back(arg->clone(keepOriginalUniqueNodeId)->castTo<FunctionParameter>());
  }
  auto clonedNode = static_cast<AbstractExpr *>(
      new Call(clonedArgs, this->getFunc()->clone(keepOriginalUniqueNodeId)->castTo<Function>()));

  clonedNode->updateClone(keepOriginalUniqueNodeId, this);
  return clonedNode;
}

void Call::setAttributes(std::vector<FunctionParameter *> functionCallParameters, Function *functionToBeCalled) {
  removeChildren();
  addChildren({new ParameterList(std::move(functionCallParameters)), functionToBeCalled});
}

int Call::getMaxNumberChildren() {
  return 2;
}

ParameterList *Call::getParameterList() const {
  return dynamic_cast<ParameterList *>(getChildAtIndex(0));
}
std::vector<std::string> Call::getVariableIdentifiers() {
  std::vector<std::string> results;
  for(auto &fp : getArguments()) {
    auto vec = fp->getVariableIdentifiers();
    if(!vec.empty()) {
      results.insert(results.end(), vec.begin(), vec.end());
    }
  }
  return results;
}

std::vector<Variable *> Call::getVariables() {
  std::vector<Variable*> results;
  for(auto &fp : getArguments()) {
    auto vec = fp->getVariables();
    if(!vec.empty()) {
      results.insert(results.end(), vec.begin(), vec.end());
    }
  }
  return results;
}

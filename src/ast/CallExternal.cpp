#include <utility>
#include <vector>
#include "ast_opt/ast/FunctionParameter.h"
#include "ast_opt/ast/CallExternal.h"

json CallExternal::toJson() const {
  json j = {
      {"type", getNodeType()},
      {"functionName", this->functionName},
  };
  // only include the field 'arguments' in JSON output if it has any value
  if (!this->arguments.empty()) {
    j["arguments"] = this->arguments;
  }
  return j;
}

CallExternal::CallExternal(std::string functionName, std::vector<FunctionParameter *> arguments)
    : functionName(std::move(functionName)) {
  this->arguments = std::move(arguments);
}

CallExternal::CallExternal(std::string functionName) : functionName(std::move(std::move(functionName))) {}

void CallExternal::accept(Visitor &v) {
  v.visit(*this);
}

const std::string &CallExternal::getFunctionName() const {
  return functionName;
}

std::string CallExternal::getNodeType() const {
  return "CallExternal";
}

CallExternal *CallExternal::clone() const {
  std::vector<FunctionParameter *> args;
  for (auto &fp : this->getArguments()) {
    args.push_back(fp->clone());
  }
  return new CallExternal(getFunctionName(), args);
}
std::vector<std::string> CallExternal::getVariableIdentifiers() {
  std::vector<std::string> results;
  for (auto &fp : getArguments()) {
    auto vec = fp->getVariableIdentifiers();
    if (!vec.empty()) {
      results.insert(results.end(), vec.begin(), vec.end());
    }
  }
  return results;
}

std::vector<Variable *> CallExternal::getVariables() {
  std::vector<Variable *> results;
  for (auto &fp : getArguments()) {
    auto vec = fp->getVariables();
    if (!vec.empty()) {
      results.insert(results.end(), vec.begin(), vec.end());
    }
  }
  return results;
}

const std::vector<FunctionParameter *> &CallExternal::getArguments() const {
  return arguments;
}

const std::vector<AbstractNode *> &CallExternal::getChildren() const {
  //TODO IMPLEMENT
  return {};
}
void CallExternal::removeChildren() {
  //TODO IMPLEMENT
}

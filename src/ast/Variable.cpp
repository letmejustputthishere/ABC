#include <utility>
#include "ast_opt/ast/Variable.h"
#include "ast_opt/ast/Ast.h"

Variable::Variable(std::string variableIdentifier) : identifier(std::move(variableIdentifier)) {}

json Variable::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["identifier"] = getIdentifier();
  return j;
}

void Variable::accept(Visitor &v) {
  v.visit(*this);
}

std::string Variable::getNodeType() const {
  return "Variable";
}

std::string Variable::getIdentifier() const {
  return identifier;
}

bool Variable::contains(Variable *var) {
  return *this==*var;
}

bool Variable::operator==(const Variable &rhs) const {
  return identifier==rhs.getIdentifier();
}

bool Variable::operator!=(const Variable &rhs) const {
  return !(rhs==*this);
}

bool Variable::isEqual(AbstractExpr *other) {
  if (auto otherVar = dynamic_cast<Variable *>(other)) {
    return *this==*otherVar;
  }
  return false;
}

std::vector<std::string> Variable::getVariableIdentifiers() {
  return {getIdentifier()};
}

std::vector<Variable *> Variable::getVariables() {
  return {this};
}

std::string Variable::toString(bool printChildren) const {
  return AbstractNode::toStringHelper(printChildren, {getIdentifier()});
}

Variable::~Variable() = default;

Variable *Variable::clone() const {
  return new Variable(getIdentifier());
}
std::vector<AbstractNode *> Variable::getChildren() {
  //TODO IMPLEMENT
  return {};
}
std::vector<const AbstractNode *> Variable::getChildren() const {
  //TODO IMPLEMENT
  return {};
}
void Variable::removeChildren() {
  //TODO IMPLEMENT
}

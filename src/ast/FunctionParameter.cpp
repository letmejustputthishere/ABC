#include "ast_opt/ast/FunctionParameter.h"

void to_json(json &j, const FunctionParameter &funcParam) {
  j = {
      {"type", funcParam.getNodeType()},
      {"value", funcParam.getValue()->toJson()},
      {"datatype", funcParam.getDatatype()->toJson()}
  };
}

void to_json(json &j, const FunctionParameter *funcParam) {
  j = {
      {"type", funcParam->getNodeType()},
      {"value", funcParam->getValue()->toJson()},
      {"datatype", funcParam->getDatatype()->toJson()}
  };
}

json FunctionParameter::toJson() const {
  json j = {
      {"type", getNodeType()},
      {"value", getValue()->toJson()},
      {"datatype", getDatatype()->toJson()}
  };
  return j;
}

void FunctionParameter::accept(Visitor &v) {
  v.visit(*this);
}

std::string FunctionParameter::getNodeType() const {
  return "FunctionParameter";
}

AbstractExpr *FunctionParameter::getValue() const {
  return dynamic_cast<AbstractExpr * >(children.at(1));
}

FunctionParameter::FunctionParameter(Datatype *datatype, AbstractExpr *value) {
  setAttributes(datatype, value);
}

FunctionParameter::FunctionParameter(const std::string &datatypeEnumString, AbstractExpr *value) {
  setAttributes(new Datatype(datatypeEnumString), value);
}

Datatype *FunctionParameter::getDatatype() const {
  return dynamic_cast<Datatype * >(children.at(0));
}

std::vector<std::string> FunctionParameter::getVariableIdentifiers() {
  return getValue()->getVariableIdentifiers();
}
std::vector<Variable *> FunctionParameter::getVariables() {
  return getValue()->getVariables();
}
FunctionParameter *FunctionParameter::clone() const {
  return new FunctionParameter(this->getDatatype()->clone(), this->getValue()->clone());
}

int FunctionParameter::getMaxNumberChildren() {
  return 2;
}

void FunctionParameter::setAttributes(Datatype *datatype, AbstractExpr *value) {
  removeChildren();
  this->datatype = datatype;
  if (datatype) datatype->setParent(this);
  this->value = value;
  if (value) value->setParent(this);
}

bool FunctionParameter::operator==(const FunctionParameter &rhs) const {
  return getValue()==rhs.getValue() && getDatatype()->getType()==rhs.getDatatype()->getType();
}

bool FunctionParameter::operator!=(const FunctionParameter &rhs) const {
  return !(rhs==*this);
}
std::string FunctionParameter::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {});
}

std::vector<AbstractNode *> FunctionParameter::getChildren()  {
  return {datatype,value};
}
void FunctionParameter::removeChildren() {
//TODO: REMOVAL
}


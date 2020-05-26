#include "ast_opt/ast/AbstractLiteral.h"
#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/LiteralString.h"
#include "ast_opt/ast/LiteralBool.h"
#include "ast_opt/ast/LiteralFloat.h"

AbstractLiteral::~AbstractLiteral() = default;

std::ostream &operator<<(std::ostream &os, const AbstractLiteral &literal) {
  literal.print(os);
  return os;
}

bool AbstractLiteral::operator==(const AbstractLiteral &rhs) const {
  // Literals cannot be equal if they have a different type
  if (typeid(*this)!=typeid(rhs)) return false;

  // cast both literals to check their equivalence
  // note: the dereference op is important here to compare the actual values, not the addresses pointed to
  if (auto thisInt = dynamic_cast<const LiteralInt *>(this)) {
    return *thisInt==*dynamic_cast<const LiteralInt *>(&rhs);
  } else if (auto thisStr = dynamic_cast<const LiteralString *>(this)) {
    return *thisStr==*dynamic_cast<const LiteralString *>(&rhs);
  } else if (auto thisBool = dynamic_cast<const LiteralBool *>(this)) {
    return *thisBool==*dynamic_cast<const LiteralBool *>(&rhs);
  } else if (auto thisFloat = dynamic_cast<const LiteralFloat *>(this)) {
    return *thisFloat==*dynamic_cast<const LiteralFloat *>(&rhs);
  } else {
    throw std::logic_error("Unexpected Literal type encountered!");
  }
}

bool AbstractLiteral::operator!=(const AbstractLiteral &rhs) const {
  return !(rhs==*this);
}

AbstractLiteral *AbstractLiteral::createLiteralBasedOnDatatype(Datatype *dt) {
  if (dt->getType()==Types::INT) {
    return new LiteralInt();
  } else if (dt->getType()==Types::BOOL) {
    return new LiteralBool();
  } else if (dt->getType()==Types::FLOAT) {
    return new LiteralFloat();
  } else if (dt->getType()==Types::STRING) {
    return new LiteralString();
  } else {
    throw std::runtime_error("AbstractLiteral::createLiteralBasedOnDatatype failed because given Datatype is unknown.");
  }
}

AbstractLiteral::AbstractLiteral(AbstractMatrix *matrix) : matrix(matrix) {}
std::vector<std::string> AbstractLiteral::getVariableIdentifiers() {
  // Literals never contain Variables
  return std::vector<std::string>();
}

std::vector<Variable *> AbstractLiteral::getVariables() {
  // Literals never contain Variables
  return std::vector<Variable *>();
}
std::vector<AbstractNode *> AbstractLiteral::getChildren()  {
  //TODO: MEMORY LEAK
  auto p = new std::vector<AbstractNode *>(1,matrix);
  return *p;
}
void AbstractLiteral::removeChildren() {
  matrix = nullptr;
}

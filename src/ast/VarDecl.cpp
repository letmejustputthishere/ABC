#include <utility>
#include "ast_opt/ast/VarDecl.h"
#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/LiteralBool.h"
#include "ast_opt/ast/LiteralFloat.h"
#include "ast_opt/ast/ArithmeticExpr.h"
#include "ast_opt/ast/Ast.h"

json VarDecl::toJson() const {
  json j = {{"type", getNodeType()},
            {"identifier", identifier},
            {"datatype", getDatatype() ? getDatatype()->toJson() : ""}};
  if (getInitializer()!=nullptr) {
    j["initializer"] = getInitializer()->toJson();
  }
  return j;
}

VarDecl::VarDecl(const std::string &, void *) {
  throw std::invalid_argument("VarDecl(std::string, AbstractExpr*) not accepted as datatype cannot be determined. "
                              "Use VarDecl(std::string, Types, AbstractExpr*) or one of the other constructors.");
}

VarDecl::VarDecl(std::string name, Datatype *datatype, AbstractExpr *initializer) {
  setAttributes(std::move(name), datatype, initializer);
}

VarDecl::VarDecl(std::string name, Types datatype, AbstractExpr *initializer) {
  setAttributes(std::move(name), new Datatype(datatype), initializer);
}

VarDecl::VarDecl(std::string name, std::string valueAssignedTo) {
  setAttributes(std::move(name),
                new Datatype(Types::STRING),
                new LiteralString(std::move(valueAssignedTo)));
}

VarDecl::VarDecl(std::string name, int valueAssignedTo) {
  setAttributes(std::move(name), new Datatype(Types::INT), new LiteralInt(valueAssignedTo));
}

VarDecl::VarDecl(std::string name, Datatype *datatype) {
  setAttributes(std::move(name), datatype, nullptr);
}

VarDecl::VarDecl(std::string name, float valueAssignedTo) {
  setAttributes(std::move(name), new Datatype(Types::FLOAT), new LiteralFloat(valueAssignedTo));
}

VarDecl::VarDecl(std::string name, bool valueAssignedTo) {
  setAttributes(std::move(name), new Datatype(Types::BOOL), new LiteralBool(valueAssignedTo));
}

VarDecl::VarDecl(std::string name, const char *valueAssignedTo)
    : VarDecl(std::move(name), std::string(valueAssignedTo)) {}

void VarDecl::setAttributes(std::string varIdentifier, Datatype *varDatatype, AbstractExpr *varValue) {
  // handle primitive attributes
  this->identifier = std::move(varIdentifier);
  // handle attributes that are itself nodes
  removeChildren();
  children = {varDatatype, varValue};
  for(auto &c: children) {
    c->setParent(this);
  }
}

void VarDecl::accept(Visitor &v) {
  v.visit(*this);
}

std::string VarDecl::getNodeType() const {
  return "VarDecl";
}

const std::string &VarDecl::getIdentifier() const {
  return identifier;
}

Datatype *VarDecl::getDatatype() const {
  return children.at(0)->castTo<Datatype>();
}

AbstractExpr *VarDecl::getInitializer() const {
  auto initializer = children.at(1);
  if (initializer==nullptr)
    return nullptr;
  return initializer->castTo<AbstractExpr>();
}

AbstractBinaryExpr *VarDecl::contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree) {
  return this->getInitializer()->contains(aexpTemplate, excludedSubtree);
}

VarDecl::~VarDecl() {
  for (auto &c : getChildren()) delete c;
}

std::string VarDecl::getVarTargetIdentifier() const {
  return this->getIdentifier();
}

bool VarDecl::isEqual(AbstractStatement *as) {
  if (auto otherVarDecl = dynamic_cast<VarDecl *>(as)) {
    return (this->getIdentifier()==otherVarDecl->getIdentifier())
        && (*this->getDatatype()==*otherVarDecl->getDatatype())
        && ((this->getInitializer()==nullptr && otherVarDecl->getInitializer()==nullptr)
            || ((this->getInitializer()!=nullptr && otherVarDecl->getInitializer()!=nullptr)
                && this->getInitializer()->isEqual(otherVarDecl->getInitializer())));
  }
  return false;
}

int VarDecl::getMaxNumberChildren() {
  return 2;
}

VarDecl *VarDecl::clone() const {
 return new VarDecl(this->getVarTargetIdentifier(),
                                this->getDatatype()->getType(),
                                getInitializer()!=nullptr
                                ? getInitializer()->clone()
                                : nullptr);
}

std::string VarDecl::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {identifier});
}

const std::vector<AbstractNode *> &VarDecl::getChildren() const {
  //TODO IMPLEMENT
  return {};
}
void VarDecl::removeChildren() {
  //TODO IMPLEMENT
}
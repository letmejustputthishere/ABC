#include <utility>
#include "ast_opt/ast/VarAssignm.h"
#include "ast_opt/ast/ArithmeticExpr.h"

json VarAssignm::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["identifier"] = this->identifier;
  j["value"] = getValue() ? getValue()->toJson() : "";
  return j;
}

VarAssignm::VarAssignm(std::string identifier, AbstractExpr *value) : identifier(std::move(identifier)) {
  setAttribute(value);
}

void VarAssignm::accept(Visitor &v) {
  v.visit(*this);
}

const std::string &VarAssignm::getIdentifier() const {
  return identifier;
}

AbstractExpr *VarAssignm::getValue() const {
  return dynamic_cast<AbstractExpr *>(getChildAtIndex(0, true));
}

std::string VarAssignm::getNodeType() const {
  return "VarAssignm";
}

AbstractBinaryExpr *VarAssignm::contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree) {
  return this->getValue()->contains(aexpTemplate, excludedSubtree);
}

std::string VarAssignm::getVarTargetIdentifier() const {
  return this->getIdentifier();
}

bool VarAssignm::isEqual(AbstractStatement *as) {
  if (auto otherVarAssignm = dynamic_cast<VarAssignm *>(as)) {
    return this->getIdentifier()==otherVarAssignm->getIdentifier() &&
        this->getValue()->isEqual(otherVarAssignm->getValue());
  }
  return false;
}

int VarAssignm::getMaxNumberChildren() {
  return 1;
}

void VarAssignm::setAttribute(AbstractExpr *assignmentValue) {
  removeChildren();
  addChildren({assignmentValue}, true);
}

VarAssignm *VarAssignm::clone(bool keepOriginalUniqueNodeId) const {
  auto clonedNode = new VarAssignm(this->getIdentifier(),
                                   this->getValue()->clone(keepOriginalUniqueNodeId)->castTo<AbstractExpr>());
  clonedNode->updateClone(keepOriginalUniqueNodeId, this);
  return clonedNode;
}

std::string VarAssignm::toString(bool printChildren) const {
  return AbstractNode::generateOutputString(printChildren, {this->identifier});
}

VarAssignm::~VarAssignm() = default;

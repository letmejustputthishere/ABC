#include "ast_opt/ast/MatrixElementRef.h"
#include "ast_opt/ast/Variable.h"

MatrixElementRef::MatrixElementRef(AbstractExpression *mustEvaluateToAbstractLiteral,
                                   int rowIndex,
                                   int columnIndex) {
  setAttributes(mustEvaluateToAbstractLiteral, new LiteralInt(rowIndex), new LiteralInt(columnIndex));
}

MatrixElementRef::MatrixElementRef(AbstractExpression *mustEvaluateToAbstractLiteral,
                                   AbstractExpression *rowIndex,
                                   AbstractExpression *columnIndex) {
  setAttributes(mustEvaluateToAbstractLiteral, rowIndex, columnIndex);
}

void MatrixElementRef::setAttributes(AbstractExpression *elementContainingMatrix,
                                     AbstractExpression *rowIndex,
                                     AbstractExpression *columnIndex) {
  removeChildren();
  children = {elementContainingMatrix, rowIndex, columnIndex};
  for(auto &c: children) {
    c->setParent(this);
  }
}

MatrixElementRef::MatrixElementRef(AbstractExpression *mustEvaluateToAbstractLiteral, AbstractExpression *rowIndex) {
  setAttributes(mustEvaluateToAbstractLiteral, rowIndex, nullptr);
}

MatrixElementRef::MatrixElementRef(AbstractExpression *mustEvaluateToAbstractLiteral, int rowIndex) {
  setAttributes(mustEvaluateToAbstractLiteral, new LiteralInt(rowIndex), nullptr);
}

std::string MatrixElementRef::getNodeType() const {
  return std::string("MatrixElementRef");
}

void MatrixElementRef::accept(Visitor &v) {
  v.visit(*this);
}

MatrixElementRef *MatrixElementRef::clone() const {
  return new MatrixElementRef(
      getOperand()->clone(),
      getRowIndex()->clone(),
      getColumnIndex()==nullptr ? nullptr :
      getColumnIndex()->clone());
}

json MatrixElementRef::toJson() const {
  json j;
  j["type"] = getNodeType();
  j["operand"] = getOperand()->toJson();
  j["rowIndex"] = getRowIndex()->toJson();
  j["columnIndex"] = getColumnIndex()->toJson();
  return j;
}

AbstractExpression *MatrixElementRef::getOperand() const {
  return dynamic_cast<AbstractExpression *>(children.at(0));
}

AbstractExpression *MatrixElementRef::getRowIndex() const {
  return dynamic_cast<AbstractExpression *>(children.at(1));
}

AbstractExpression *MatrixElementRef::getColumnIndex() const {
  return dynamic_cast<AbstractExpression *>(children.at(2));
}

std::vector<std::string> MatrixElementRef::getVariableIdentifiers() {
  std::vector<std::string> resultVec;
  auto insertIfNonEmpty = [&resultVec](std::vector<std::string> vec) {
    if (!vec.empty()) {
      resultVec.insert(resultVec.end(), vec.begin(), vec.end());
    } else {
      resultVec.emplace_back("");
    }
  };
  if (getOperand()) {
    insertIfNonEmpty(getOperand()->getVariableIdentifiers());
  } else {
    throw std::logic_error("Matrix Element Ref does not have a Variable Identifier.");
  };
  if (getRowIndex()) {
    insertIfNonEmpty(getRowIndex()->getVariableIdentifiers());
  } else {
    resultVec.emplace_back("");
  }
  if (getColumnIndex()) {
    insertIfNonEmpty(getColumnIndex()->getVariableIdentifiers());
  } else {
    resultVec.emplace_back("");
  }
  return resultVec;
}

std::vector<Variable *> MatrixElementRef::getVariables() {
  std::vector<Variable *> resultVec;
  auto insertIfNonEmpty = [&resultVec](std::vector<Variable *> vec) {
    if (!vec.empty()) resultVec.insert(resultVec.end(), vec.begin(), vec.end());
  };
  insertIfNonEmpty(getOperand()->getVariables());
  insertIfNonEmpty(getRowIndex()->getVariables());
  insertIfNonEmpty(getColumnIndex()->getVariables());
  return resultVec;
}

bool MatrixElementRef::contains(Variable *var) {
  auto vars = getVariableIdentifiers();
  return std::count(vars.begin(), vars.end(), var->getIdentifier()) > 0;
}

bool MatrixElementRef::isEqual(AbstractExpression *other) {
  if (auto otherAsGME = dynamic_cast<MatrixElementRef *>(other)) {
    return getOperand()->isEqual(otherAsGME->getOperand())
        && getRowIndex()->isEqual(otherAsGME->getRowIndex())
        && getColumnIndex()->isEqual(otherAsGME->getColumnIndex());
  }
  return false;
}

int MatrixElementRef::getMaxNumberChildren() {
  return 3;
}

std::string MatrixElementRef::toString(bool printChildren) const {
  return AbstractNode::toStringHelper(printChildren, {});
}

std::vector<AbstractNode *> MatrixElementRef::getChildren()  {
  //TODO IMPLEMENT
  return {};
}
std::vector<const AbstractNode *> MatrixElementRef::getChildren() const {
  //TODO IMPLEMENT
  return {};
}
void MatrixElementRef::removeChildren() {
  //TODO IMPLEMENT
}

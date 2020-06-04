#include "ast_opt/ast/AbstractMatrix.h"
#include "ast_opt/ast/Matrix.h"

bool AbstractMatrix::operator==(const AbstractMatrix &rhs) const {
  // Matrices cannot be equal if they have a different type
  if (typeid(*this)!=typeid(rhs)) return false;

  // cast both AbstractMatrices to check their equivalence
  // note: the dereference op is important here to compare the actual values, not the addresses pointed to
  if (auto thisAsInt = dynamic_cast<const Matrix<int> *>(this)) {  // Matrix<int>
    return *thisAsInt==*dynamic_cast<const Matrix<int> *>(&rhs);
  } else if (auto thisAsFloat = dynamic_cast<const Matrix<float> *>(this)) {  // Matrix<float>
    return *thisAsFloat==*dynamic_cast<const Matrix<float> *>(&rhs);
  } else if (auto thisAsBool = dynamic_cast<const Matrix<bool> *>(this)) {  // Matrix<bool>
    return *thisAsBool==*dynamic_cast<const Matrix<bool> *>(&rhs);
  } else if (auto thisAsString = dynamic_cast<const Matrix<std::string> *>(this)) {  // Matrix<std::string>
    return *thisAsString==*dynamic_cast<const Matrix<std::string> *>(&rhs);
  } else if (auto thisAsAbstractExpr = dynamic_cast<const Matrix<AbstractExpression *> *>(this)) {  // Matrix<AbstractExpr*>
    return *thisAsAbstractExpr==*dynamic_cast<const Matrix<AbstractExpression *> *>(&rhs);
  } else {
    throw std::logic_error("Could not determine type T of AbstractMatrix (e.g., Matrix<int>). Aborting.");
  }
}

bool AbstractMatrix::operator!=(const AbstractMatrix &rhs) const {
  return !(rhs==*this);
}
AbstractNode::iterator AbstractMatrix::begin() {
  //TODO
  return AbstractNode::iterator();
}
AbstractNode::const_iterator AbstractMatrix::begin() const {
  //TODO
  return AbstractNode::const_iterator();
}
AbstractNode::iterator AbstractMatrix::end() {
  //TODO
  return AbstractNode::iterator();
}
AbstractNode::const_iterator AbstractMatrix::end() const {
  //TODO
  return AbstractNode::const_iterator();
}

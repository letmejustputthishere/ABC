#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_MATRIXELEMENTREF_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_MATRIXELEMENTREF_H_

#include "AbstractLiteral.h"
#include <vector>
#include <string>
#include <unordered_map>

class MatrixElementRef : public AbstractExpression {
 public:
  MatrixElementRef(AbstractExpression *mustEvaluateToAbstractLiteral, AbstractExpression *rowIndex, AbstractExpression *columnIndex);

  MatrixElementRef(AbstractExpression *mustEvaluateToAbstractLiteral, AbstractExpression *rowIndex);

  MatrixElementRef(AbstractExpression *mustEvaluateToAbstractLiteral, int rowIndex, int columnIndex);

  MatrixElementRef(AbstractExpression *mustEvaluateToAbstractLiteral, int rowIndex);

  [[nodiscard]] std::string getNodeType() const override;

  void accept(Visitor &v) override;

  MatrixElementRef *clone() const override;

  [[nodiscard]] json toJson() const override;

  std::vector<std::string> getVariableIdentifiers() override;

  std::vector<Variable *> getVariables() override;

  bool contains(Variable *var) override;

  bool isEqual(AbstractExpression *other) override;

  int getMaxNumberChildren() override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  [[nodiscard]] AbstractExpression *getOperand() const;

  [[nodiscard]] AbstractExpression *getRowIndex() const;

  [[nodiscard]] AbstractExpression *getColumnIndex() const;

  void setAttributes(AbstractExpression *elementContainingMatrix, AbstractExpression *rowIndex, AbstractExpression *columnIndex);
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_MATRIXELEMENTREF_H_

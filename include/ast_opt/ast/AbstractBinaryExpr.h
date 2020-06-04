#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTBINARYEXPR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTBINARYEXPR_H_

#include "AbstractExpression.h"

class AbstractBinaryExpr : public AbstractExpression {
 public:
  void setAttributes(AbstractExpression *leftOperand, Operator *operatore, AbstractExpression *rightOperand);

  [[nodiscard]] virtual AbstractExpression *getLeft() const = 0;

  [[nodiscard]] virtual Operator *getOperator() const = 0;

  [[nodiscard]] virtual AbstractExpression *getRight() const = 0;

  virtual void setLeft(AbstractExpression *newleft) = 0;

  virtual void setOperator(Operator *newOperator) = 0;

  virtual void setRight(AbstractExpression *newRight) = 0;

  int getMaxNumberChildren() override;

  [[nodiscard]] json toJson() const override;

  std::vector<std::string> getVariableIdentifiers() override;

  bool contains(Variable *var) override;

  int countByTemplate(AbstractExpression *abstractExpr) override;

  AbstractBinaryExpr *contains(AbstractBinaryExpr *aexpTemplate, AbstractExpression *excludedSubtree) override;

  bool isEqual(AbstractExpression *other) override;

  static void swapOperandsLeftAWithRightB(AbstractBinaryExpr *aexpA, AbstractBinaryExpr *aexpB);

  std::vector<Variable *> getVariables() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTBINARYEXPR_H_

#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTBINARYEXPR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTBINARYEXPR_H_

#include "AbstractExpr.h"

class AbstractBinaryExpr : public AbstractExpr {
 public:
  void setAttributes(AbstractExpr *leftOperand, Operator *operatore, AbstractExpr *rightOperand);

  [[nodiscard]] virtual AbstractExpr *getLeft() const = 0;

  [[nodiscard]] virtual Operator *getOperator() const = 0;

  [[nodiscard]] virtual AbstractExpr *getRight() const = 0;

  virtual void setLeft(AbstractExpr *newleft) = 0;

  virtual void setOperator(Operator *newOperator) = 0;

  virtual void setRight(AbstractExpr *newRight) = 0;

  int getMaxNumberChildren() override;

  [[nodiscard]] json toJson() const override;

  std::vector<std::string> getVariableIdentifiers() override;

  bool contains(Variable *var) override;

  int countByTemplate(AbstractExpr *abstractExpr) override;

  AbstractBinaryExpr *contains(AbstractBinaryExpr *aexpTemplate, AbstractExpr *excludedSubtree) override;

  bool isEqual(AbstractExpr *other) override;

  static void swapOperandsLeftAWithRightB(AbstractBinaryExpr *aexpA, AbstractBinaryExpr *aexpB);

  std::vector<Variable *> getVariables() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTBINARYEXPR_H_

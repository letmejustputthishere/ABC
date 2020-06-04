#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOREXPR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOREXPR_H_

#include "AbstractExpression.h"
#include <string>
#include <vector>

class OperatorExpr : public AbstractExpression {
 private:
  Operator* op;
  std::vector<AbstractExpression*> operands;
 public:
  OperatorExpr();

  explicit OperatorExpr(Operator *op);

  OperatorExpr(Operator *op, std::vector<AbstractExpression *> operands);

  OperatorExpr *clone() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  int getMaxNumberChildren() override;

  void setOperator(Operator *op);

  void addOperand(AbstractExpression *operand);

  void addOperands(std::vector<AbstractExpression *> operands);

  void removeOperand(AbstractExpression * operand);

  [[nodiscard]] Operator *getOperator() const;

  [[nodiscard]] std::vector<AbstractExpression *> getOperands() const;

  // Methods for backwards compatibility to AbstractBinaryExpr

  [[nodiscard]] AbstractExpression *getRight() const;

  [[nodiscard]] AbstractExpression *getLeft() const;



  void setAttributes(Operator *newOperator, std::vector<AbstractExpression *> newOperands);

  [[nodiscard]] bool isLogicalExpr() const;

  [[nodiscard]] bool isArithmeticExpr() const;

  [[nodiscard]] bool isUnaryExpr() const;

  OperatorExpr(AbstractExpression *lhsOperand, Operator *op, AbstractExpression *rhsOperand);

  bool isEqual(AbstractExpression *other) override;

  void replaceChild(AbstractNode *originalChild, AbstractNode *newChildToBeAdded) override;

  std::vector<std::string> getVariableIdentifiers() override;

  std::vector<Variable *> getVariables() override;
  std::vector<AbstractNode *> getChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
  void removeChildren() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOREXPR_H_

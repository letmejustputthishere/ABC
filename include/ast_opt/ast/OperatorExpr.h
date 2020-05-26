#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOREXPR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOREXPR_H_

#include "AbstractExpr.h"
#include <string>
#include <vector>

class OperatorExpr : public AbstractExpr {
 private:
  Operator* op;
  std::vector<AbstractExpr*> operands;
 public:
  OperatorExpr();

  explicit OperatorExpr(Operator *op);

  OperatorExpr(Operator *op, std::vector<AbstractExpr *> operands);

  OperatorExpr *clone() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  int getMaxNumberChildren() override;

  void setOperator(Operator *op);

  void addOperand(AbstractExpr *operand);

  void removeOperand(AbstractExpr * operand);

  [[nodiscard]] Operator *getOperator() const;

  [[nodiscard]] std::vector<AbstractExpr *> getOperands() const;

  // Methods for backwards compatibility to AbstractBinaryExpr

  [[nodiscard]] AbstractExpr *getRight() const;

  [[nodiscard]] AbstractExpr *getLeft() const;



  void setAttributes(Operator *newOperator, std::vector<AbstractExpr *> newOperands);

  [[nodiscard]] bool isLogicalExpr() const;

  [[nodiscard]] bool isArithmeticExpr() const;

  [[nodiscard]] bool isUnaryExpr() const;

  OperatorExpr(AbstractExpr *lhsOperand, Operator *op, AbstractExpr *rhsOperand);

  bool isEqual(AbstractExpr *other) override;

  void replaceChild(AbstractNode *originalChild, AbstractNode *newChildToBeAdded) override;

  std::vector<std::string> getVariableIdentifiers() override;

  std::vector<Variable *> getVariables() override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOREXPR_H_

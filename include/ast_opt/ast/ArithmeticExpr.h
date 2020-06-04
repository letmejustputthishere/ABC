#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ARITHMETICEXPR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ARITHMETICEXPR_H_

#include <string>
#include <vector>
#include "Operator.h"
#include "AbstractExpression.h"
#include "AbstractLiteral.h"
#include "LiteralInt.h"
#include "LiteralBool.h"
#include "LiteralString.h"
#include "AbstractBinaryExpr.h"

class ArithmeticExpr : public AbstractBinaryExpr {
 private:
  AbstractExpression * left;
  Operator* op;
  AbstractExpression * right;
 public:
  /// Represents an expression of the form "left op right", e.g., "2 + a" or "53 * 3".
  /// \param left is the left operand of the expression.
  /// \param op is the operator of the expression.
  /// \param right is the right operand of the expression.
  ArithmeticExpr(AbstractExpression *left, ArithmeticOp op, AbstractExpression *right);

  ArithmeticExpr();

  explicit ArithmeticExpr(ArithmeticOp op);

  ArithmeticExpr *clone() const override;

  template<typename T1, typename T2>
  ArithmeticExpr(T1 left, ArithmeticOp op, T2 right) {
    setAttributes(AbstractExpression::createParam(left), new Operator(op), AbstractExpression::createParam(right));
  }

  template<typename T1, typename T2>
  ArithmeticExpr(T1 left, Operator *op, T2 right) {
    setAttributes(AbstractExpression::createParam(left), op, AbstractExpression::createParam(right));
  }

  ~ArithmeticExpr() override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  void setLeft(AbstractExpression* left) override;

  void setOperator(Operator *newOperator) override;
  void setRight(AbstractExpression *newRight) override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  AbstractExpression *getLeft() const override;
  Operator *getOperator() const override;
  AbstractExpression *getRight() const override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ARITHMETICEXPR_H_

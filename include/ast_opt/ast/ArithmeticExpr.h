#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ARITHMETICEXPR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ARITHMETICEXPR_H_

#include <string>
#include <vector>
#include "Operator.h"
#include "AbstractExpr.h"
#include "AbstractLiteral.h"
#include "LiteralInt.h"
#include "LiteralBool.h"
#include "LiteralString.h"
#include "AbstractBinaryExpr.h"

class ArithmeticExpr : public AbstractBinaryExpr {
 private:
  AbstractExpr * left;
  Operator* op;
  AbstractExpr * right;
 public:
  /// Represents an expression of the form "left op right", e.g., "2 + a" or "53 * 3".
  /// \param left is the left operand of the expression.
  /// \param op is the operator of the expression.
  /// \param right is the right operand of the expression.
  ArithmeticExpr(AbstractExpr *left, ArithmeticOp op, AbstractExpr *right);

  ArithmeticExpr();

  explicit ArithmeticExpr(ArithmeticOp op);

  ArithmeticExpr *clone() const override;

  template<typename T1, typename T2>
  ArithmeticExpr(T1 left, ArithmeticOp op, T2 right) {
    setAttributes(AbstractExpr::createParam(left), new Operator(op), AbstractExpr::createParam(right));
  }

  template<typename T1, typename T2>
  ArithmeticExpr(T1 left, Operator *op, T2 right) {
    setAttributes(AbstractExpr::createParam(left), op, AbstractExpr::createParam(right));
  }

  ~ArithmeticExpr() override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  void setLeft(AbstractExpr* left) override;

  void setOperator(Operator *newOperator) override;
  void setRight(AbstractExpr *newRight) override;
  const std::vector<AbstractNode *> &getChildren() const override;
  void removeChildren() override;
  AbstractExpr *getLeft() const override;
  Operator *getOperator() const override;
  AbstractExpr *getRight() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ARITHMETICEXPR_H_

#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_LOGICALEXPR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_LOGICALEXPR_H_

#include "Operator.h"
#include "AbstractExpr.h"
#include "AbstractLiteral.h"
#include "LiteralInt.h"
#include "LiteralBool.h"
#include "LiteralString.h"
#include "AbstractBinaryExpr.h"
#include <string>
#include <vector>

class LogicalExpr : public AbstractBinaryExpr {
 private:
  AbstractExpr *left;
  Operator *op;
  AbstractExpr *right;
 public:
  LogicalExpr();

  explicit LogicalExpr(LogCompOp op);

  template<typename T1, typename T2>
  LogicalExpr(T1 left, LogCompOp op, T2 right) {
    setAttributes(AbstractExpr::createParam(left), new Operator(op), AbstractExpr::createParam(right));
  }

  template<typename T1, typename T2>
  LogicalExpr(T1 left, Operator *op, T2 right) {
    setAttributes(AbstractExpr::createParam(left), op, AbstractExpr::createParam(right));
  }

  LogicalExpr *clone() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  AbstractExpr *getLeft() const override;
  void setLeft(AbstractExpr *newleft) override;
  void setOperator(Operator *newOperator) override;
  void setRight(AbstractExpr *newRight) override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  Operator *getOperator() const override;
  AbstractExpr *getRight() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_LOGICALEXPR_H_

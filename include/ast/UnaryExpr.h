#ifndef AST_OPTIMIZER_UNARYEXPR_H
#define AST_OPTIMIZER_UNARYEXPR_H

#include <string>
#include "AbstractExpr.h"
#include "Operator.h"

class UnaryExpr : public AbstractExpr {
 private:
//  Operator* op;
//  AbstractExpr* right;

 public:
  UnaryExpr(OpSymb::UnaryOp op, AbstractExpr* right);

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] Operator* getOp() const;

  [[nodiscard]] AbstractExpr* getRight() const;

  [[nodiscard]] std::string getNodeName() const override;

  ~UnaryExpr() override;

  Literal* evaluate(Ast &ast) override;
 protected:
  bool supportsCircuitMode() override;
  int getMaxNumberChildren() override;
  void setAttributes(OpSymb::UnaryOp op, AbstractExpr* expr);
};

#endif //AST_OPTIMIZER_UNARYEXPR_H

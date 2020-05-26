#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_UNARYEXPR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_UNARYEXPR_H_

#include <string>
#include "AbstractExpr.h"
#include "Operator.h"

class UnaryExpr : public AbstractExpr {
 public:
  UnaryExpr(UnaryOp op, AbstractExpr *right);

  UnaryExpr *clone(bool keepOriginalUniqueNodeId) const override;

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] Operator *getOperator() const;

  void setOperator(Operator* newOperator);

  [[nodiscard]] AbstractExpr *getRight() const;

  [[nodiscard]] std::string getNodeType() const override;

  ~UnaryExpr() override;

  void setAttributes(UnaryOp op, AbstractExpr *expr);

  [[nodiscard]] std::string toString(bool printChildren) const override;

  bool isEqual(AbstractExpr *other) override;

  std::vector<std::string> getVariableIdentifiers() override;

  std::vector<Variable *> getVariables() override;

 protected:

  int getMaxNumberChildren() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_UNARYEXPR_H_

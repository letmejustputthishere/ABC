#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_RETURN_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_RETURN_H_

#include <string>
#include <vector>
#include "AbstractStatement.h"
#include "AbstractExpression.h"

class Return : public AbstractStatement {
 private:
  std::vector<AbstractExpression*> returnExpressions;
 public:
  Return();

  explicit Return(AbstractExpression *returnValue);

  explicit Return(std::vector<AbstractExpression *> returnValues);

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  ~Return() override;

  [[nodiscard]] std::vector<AbstractExpression *> getReturnExpressions() const;

  Return *clone() const override;

  void setAttributes(std::vector<AbstractExpression *> returnExpr);

  void addReturnExpr(AbstractExpression* returnExpr);

  [[nodiscard]] std::string toString(bool printChildren) const override;
  bool isEqual(const AbstractNode *as) const override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;

 protected:
  int getMaxNumberChildren() override;

};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_RETURN_H_

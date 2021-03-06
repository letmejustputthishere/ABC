#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_RETURN_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_RETURN_H_

#include <string>
#include <vector>
#include "AbstractStatement.h"
#include "AbstractExpr.h"

class Return : public AbstractStatement {
 public:
  Return();

  explicit Return(AbstractExpr *returnValue);

  explicit Return(std::vector<AbstractExpr *> returnValues);

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  ~Return() override;

  [[nodiscard]] std::vector<AbstractExpr *> getReturnExpressions() const;

  Return *clone(bool keepOriginalUniqueNodeId) override;

  void setAttributes(std::vector<AbstractExpr *> returnExpr);

  [[nodiscard]] std::string toString(bool printChildren) const override;
  bool isEqual(AbstractStatement *as) override;

 protected:
  int getMaxNumberChildren() override;

  bool supportsCircuitMode() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_RETURN_H_

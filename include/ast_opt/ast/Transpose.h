#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_TRANSPOSE_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_TRANSPOSE_H_

#include "AbstractExpression.h"
#include <vector>
#include <string>

class Transpose : public AbstractExpression {
 public:
  Transpose();

  explicit Transpose(AbstractExpression *operand);

  [[nodiscard]] std::string getNodeType() const override;

  void accept(Visitor &v) override;

  Transpose *clone() const override;

  [[nodiscard]] json toJson() const override;

  std::vector<std::string> getVariableIdentifiers() override;
  
  std::vector<Variable *> getVariables() override;

  bool contains(Variable *var) override;

  bool isEqual(AbstractExpression *other) override;

  int getMaxNumberChildren() override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  [[nodiscard]] AbstractExpression *getOperand() const;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_TRANSPOSE_H_

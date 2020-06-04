#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ROTATE_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ROTATE_H_

#include <string>
#include "AbstractExpression.h"
#include "AbstractLiteral.h"
#include "LiteralBool.h"
#include "LiteralString.h"
#include "LiteralFloat.h"
#include "LiteralInt.h"
#include "Variable.h"

class Rotate : public AbstractExpression {
 public:
  Rotate();

  Rotate(AbstractExpression *vector, AbstractExpression *rotationFactor);

  Rotate(AbstractExpression *vector, int rotationFactor);

  [[nodiscard]] AbstractExpression *getRotationFactor() const;

  int getMaxNumberChildren() override;

  [[nodiscard]] json toJson() const override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  [[nodiscard]] AbstractExpression *getOperand() const;

  [[nodiscard]] std::string getNodeType() const override;

  void accept(Visitor &v) override;

  Rotate *clone() const override;

  void setAttributes(AbstractExpression *pExpr, AbstractExpression *rotationFactor);

  static bool isOneDimensionalVector(AbstractExpression *operand);

  std::vector<std::string> getVariableIdentifiers() override;

  std::vector<Variable *> getVariables() override;
  
  bool isEqual(AbstractExpression *other) override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ROTATE_H_

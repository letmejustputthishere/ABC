#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ROTATE_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ROTATE_H_

#include <string>
#include "AbstractExpr.h"
#include "AbstractLiteral.h"
#include "LiteralBool.h"
#include "LiteralString.h"
#include "LiteralFloat.h"
#include "LiteralInt.h"
#include "Variable.h"

class Rotate : public AbstractExpr {
 public:
  Rotate();

  Rotate(AbstractExpr *vector, AbstractExpr *rotationFactor);

  Rotate(AbstractExpr *vector, int rotationFactor);

  [[nodiscard]] AbstractExpr *getRotationFactor() const;

  int getMaxNumberChildren() override;

  [[nodiscard]] json toJson() const override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  AbstractNode *cloneFlat() override;

  bool supportsCircuitMode() override;

  [[nodiscard]] AbstractExpr *getOperand() const;

  [[nodiscard]] std::string getNodeType() const override;

  void accept(Visitor &v) override;

  Rotate *clone(bool keepOriginalUniqueNodeId) override;

  void setAttributes(AbstractExpr *pExpr, AbstractExpr *rotationFactor);

  static bool isOneDimensionalVector(AbstractExpr *operand);
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ROTATE_H_

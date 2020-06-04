#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARASSIGNM_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARASSIGNM_H_

#include <string>
#include "AbstractStatement.h"
#include "AbstractExpression.h"

class VarAssignm : public AbstractStatement {
 private:
  std::string identifier;

 public:
  VarAssignm(std::string identifier, AbstractExpression *value);

  ~VarAssignm() override;

  VarAssignm *clone() const override;

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] const std::string &getIdentifier() const;

  [[nodiscard]] AbstractExpression *getValue() const;

  [[nodiscard]] std::string getNodeType() const override;

  AbstractBinaryExpr *contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree) override;

  std::string getVarTargetIdentifier() const;

  bool isEqual(const AbstractNode *as) const override;

  int getMaxNumberChildren() override;

  void setAttribute(AbstractExpression *assignmentValue);

  [[nodiscard]] std::string toString(bool printChildren) const override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARASSIGNM_H_

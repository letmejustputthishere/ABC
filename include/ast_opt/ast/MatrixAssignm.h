#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_MATRIXASSIGNM_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_MATRIXASSIGNM_H_

#include "AbstractStatement.h"
#include "ast_opt/ast/MatrixElementRef.h"
#include <string>

class MatrixAssignm : public AbstractStatement {
 public:
  MatrixAssignm(MatrixElementRef *assignmentTarget, AbstractExpression *value);

  ~MatrixAssignm() override;

  void setAttributes(AbstractExpression *assignmTarget, AbstractExpression *value);

  [[nodiscard]] std::string getNodeType() const override;

  int getMaxNumberChildren() override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  MatrixAssignm *clone() const override;

  [[nodiscard]] json toJson() const override;

  AbstractBinaryExpr *contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree) override;

  bool isEqual(const AbstractNode *as) const override;

  [[nodiscard]] MatrixElementRef *getAssignmTarget() const;

  [[nodiscard]] AbstractExpression *getValue() const;

  [[nodiscard]] std::string getAssignmTargetString() const;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_MATRIXASSIGNM_H_

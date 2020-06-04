#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_WHILE_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_WHILE_H_

#include "AbstractStatement.h"
#include "AbstractExpression.h"
#include <string>

class While : public AbstractStatement {
 public:
  While(AbstractExpression *condition, AbstractStatement *body);

  While *clone() const override;

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] AbstractExpression *getCondition() const;

  [[nodiscard]] AbstractStatement *getBody() const;

  [[nodiscard]] std::string getNodeType() const override;

  void setAttributes(AbstractExpression *loopCondition, AbstractStatement *loopBody);

  int getMaxNumberChildren() override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;

};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_WHILE_H_

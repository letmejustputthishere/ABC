#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_FOR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_FOR_H_

#include "AbstractStatement.h"
#include <string>

class For : public AbstractStatement {
 public:
  For(AbstractStatement *initializer,
      AbstractExpr *condition,
      AbstractStatement *update,
      AbstractStatement *statementToBeExecuted);

  [[nodiscard]] std::string getNodeType() const override;

  void accept(Visitor &v) override;

  For *clone() const override;

  void setAttributes(AbstractStatement *initializer,
                     AbstractExpr *condition,
                     AbstractStatement *update,
                     AbstractStatement *body);

  [[nodiscard]] Block *getInitializer() const;

  void setInitializer(Block *initializer);

  void setBody(Block *body);

  [[nodiscard]] AbstractExpr *getCondition() const;

  [[nodiscard]] Block *getUpdate() const;

  [[nodiscard]] Block *getBody() const;

  int getMaxNumberChildren() override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  [[nodiscard]] json toJson() const override;

  bool isEqual(const AbstractNode *other) const override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_FOR_H_

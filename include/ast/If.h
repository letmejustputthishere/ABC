#ifndef AST_OPTIMIZER_IF_H
#define AST_OPTIMIZER_IF_H

#include "AbstractStatement.h"
#include "AbstractExpr.h"
#include <string>

class If : public AbstractStatement {
 private:
  AbstractExpr* condition;
  AbstractStatement* thenBranch;
  AbstractStatement* elseBranch;

 public:
  If(AbstractExpr* condition, AbstractStatement* thenBranch);

  If(AbstractExpr* condition, AbstractStatement* thenBranch, AbstractStatement* elseBranch);

  ~If();

  [[nodiscard]] json toJson() const override;

  void accept(IVisitor &v) override;

  [[nodiscard]] std::string getNodeName() const override;

  [[nodiscard]] AbstractExpr* getCondition() const;

  [[nodiscard]] AbstractStatement* getThenBranch() const;

  [[nodiscard]] AbstractStatement* getElseBranch() const;

  Literal* evaluate(Ast &ast) override;
  void updateTree();
};

#endif //AST_OPTIMIZER_IF_H

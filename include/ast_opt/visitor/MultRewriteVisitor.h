#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_VISITOR_MULTREWRITEVISITOR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_VISITOR_MULTREWRITEVISITOR_H_

#include "ast_opt/visitor/Visitor.h"
#include "ast_opt/utilities/Scope.h"

/// This visitor is an example for an AST rewrite operation.
class MultRewriteVisitor : public Visitor {
 private:
  int numChanges{};

 public:
  [[nodiscard]] int getNumChanges() const;

  [[nodiscard]] bool changedAst() const;

  MultRewriteVisitor() = default;

  void visit(ArithmeticExpr &elem) override;

  void visit(Ast &elem) override;

  void visit(OperatorExpr &elem) override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_VISITOR_MULTREWRITEVISITOR_H_

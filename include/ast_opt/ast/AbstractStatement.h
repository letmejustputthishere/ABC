#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTSTATEMENT_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTSTATEMENT_H_
#include "ast_opt/ast/AbstractNode.h"
class AbstractStatement : public AbstractNode {
 public:
  ///TODO: What does this even do?
  virtual AbstractBinaryExpr *contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree) = 0;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTSTATEMENT_H_

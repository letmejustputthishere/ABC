#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_CALL_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_CALL_H_

#include <string>
#include <vector>
#include "AbstractExpression.h"
#include "FunctionParameter.h"
#include "AbstractStatement.h"

class Call : public AbstractExpression {
 private:
  ParameterList * arguments;
  Function* func;
 public:
  Call(std::vector<FunctionParameter *> parameterValuesForCalledFunction, Function *func);

  explicit Call(Function *func);

  Call *clone() const override;

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::vector<FunctionParameter *> getArguments() const;

  [[nodiscard]] ParameterList *getParameterList() const;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] Function *getFunc() const;

  void setAttributes(std::vector<FunctionParameter *> functionCallParameters, Function *functionToBeCalled);

  int getMaxNumberChildren() override;

  std::vector<std::string> getVariableIdentifiers() override;

  std::vector<Variable *> getVariables() override;
  AbstractBinaryExpr *contains(AbstractBinaryExpr *aexpTemplate, AbstractExpression *excludedSubtree) override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_CALL_H_

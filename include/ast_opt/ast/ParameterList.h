#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_PARAMETERLIST_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_PARAMETERLIST_H_

#include "AbstractStatement.h"

class ParameterList : public AbstractStatement {
 public:
  ParameterList() = default;

  explicit ParameterList(std::vector<FunctionParameter*> parameters);

  [[nodiscard]] std::string getNodeType() const override;

  void accept(Visitor &v) override;

  ParameterList *clone() const override;

  std::vector<FunctionParameter*> getParameters();

  int getMaxNumberChildren() override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  void addParameter(FunctionParameter* param);
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_PARAMETERLIST_H_

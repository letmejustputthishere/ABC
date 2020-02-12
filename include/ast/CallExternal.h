#ifndef AST_OPTIMIZER_CALLEXTERNAL_H
#define AST_OPTIMIZER_CALLEXTERNAL_H

#include "AbstractStatement.h"
#include <vector>
#include <string>

class CallExternal : public AbstractExpr, public AbstractStatement {
 private:
  std::string functionName;
  std::vector<FunctionParameter>* arguments;

 public:
  explicit CallExternal(std::string functionName);

  CallExternal(std::string functionName, std::vector<FunctionParameter>* arguments);

  [[nodiscard]] json toJson() const override;

  void accept(IVisitor &v) override;

  [[nodiscard]] const std::string &getFunctionName() const;

  [[nodiscard]] std::vector<FunctionParameter>* getArguments() const;

  [[nodiscard]] std::string getNodeName() const override;

  Literal* evaluate(Ast &ast) override;
};

#endif //AST_OPTIMIZER_CALLEXTERNAL_H

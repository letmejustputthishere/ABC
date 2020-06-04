#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARIABLE_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARIABLE_H_

#include <map>
#include <string>
#include <vector>
#include "AbstractExpression.h"
#include "Matrix.h"

class Variable : public AbstractExpression {
 private:
  std::string identifier;

 public:
  explicit Variable(std::string identifier);

  [[nodiscard]] json toJson() const override;

  ~Variable() override;

  Variable *clone() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] std::string getIdentifier() const;

  bool operator==(const Variable &rhs) const;

  bool operator!=(const Variable &rhs) const;

  bool contains(Variable *var) override;

  bool isEqual(AbstractExpression *other) override;

  std::vector<std::string> getVariableIdentifiers() override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  std::vector<Variable *> getVariables() override;
  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARIABLE_H_

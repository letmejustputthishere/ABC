#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTSTATEMENT_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTSTATEMENT_H_

#include <string>
#include <nlohmann/json.hpp>
#include <vector>
#include <map>
#include <sstream>
#include <typeinfo>
#include "ast_opt/visitor/Visitor.h"
#include "ast_opt/ast/AbstractNode.h"
#include "ast_opt/ast/AbstractBinaryExpr.h"

using json = nlohmann::json;

class AbstractStatement : public AbstractNode {
 public:
  [[nodiscard]] json toJson() const override;

  virtual AbstractBinaryExpr *contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree);

  virtual bool isEqual(AbstractStatement *as);

  virtual AbstractStatement* clone() const override = 0;
 protected:
/// Stores the children nodes of the current node.
std::vector<AbstractNode *> children{};
};

std::ostream &operator<<(std::ostream &outs, const AbstractStatement &obj);

/// JSON representation to be used for vector<AbstractStatement> objects.
void to_json(json &j, const AbstractStatement &absStat);

void to_json(json &j, const AbstractStatement *absStat);

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTSTATEMENT_H_

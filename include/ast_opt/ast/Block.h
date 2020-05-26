#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_BLOCK_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_BLOCK_H_

#include <vector>
#include "AbstractStatement.h"
#include <string>

class Block : public AbstractStatement {
 public:
  Block() = default;

  explicit Block(std::vector<AbstractStatement *> statements);

  ~Block() override = default;

  Block *clone() const override;

  explicit Block(AbstractStatement *stat);

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] std::vector<AbstractStatement *> getStatements();

  [[nodiscard]] std::vector<const AbstractStatement *> getStatements() const;

  int getMaxNumberChildren() override;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  bool isEqual(AbstractStatement *otherBlockStatement) override;

  void addStatement(AbstractStatement *stmt);

  std::vector<AbstractNode *> getChildren() override;

  std::vector<const AbstractNode *> getChildren() const override;
  void removeChildren() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_BLOCK_H_

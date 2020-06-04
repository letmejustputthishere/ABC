#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTLITERAL_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTLITERAL_H_

#include <variant>
#include <ostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "AbstractExpression.h"
#include "Datatype.h"
#include "ast_opt/ast/AbstractMatrix.h"

class RandLiteralGen;


class AbstractLiteral : public AbstractExpression {
 protected:
  /// Stores the values of this Literal subtype. For example, for LiteralInt It can be a Matrix<int> but can also be a
  /// Matrix<AbstractExpr*> in the case that this matrix contains unevaluated expression, e.g., MatrixElementRef.
  AbstractMatrix *matrix;

  explicit AbstractLiteral(AbstractMatrix *matrix);

  ~AbstractLiteral() override;

  virtual void print(std::ostream &str) const = 0;

 public:
  friend std::ostream &operator<<(std::ostream &os, const AbstractLiteral &literal);

  bool operator==(const AbstractLiteral &rhs) const;

  bool operator!=(const AbstractLiteral &rhs) const;

  virtual void addLiteralValue(std::string identifier,
                               std::unordered_map<std::string, const AbstractLiteral *> &paramsMap) = 0;

  virtual void setRandomValue(RandLiteralGen &rlg) = 0;

  virtual bool supportsDatatype(const Datatype &datatype) const = 0;

  virtual bool isNull() = 0;

  [[nodiscard]] virtual AbstractMatrix *getMatrix() const = 0;

  virtual void setMatrix(AbstractMatrix *newValue) = 0;

  /// Creates a new and valueless literal matching the given datatype. For example, Types::INT would create a new
  /// LiteralInt and Types::BOOL a new LiteralBool.
  /// \param dt A pointer to a Datatype object.
  /// \return The created AbstractLiteral subtype instance.
  static AbstractLiteral *createLiteralBasedOnDatatype(Datatype *dt);

  std::vector<std::string> getVariableIdentifiers() override;

  std::vector<Variable *> getVariables() override;

  std::vector<AbstractNode *> getChildren() override;

  std::vector<const AbstractNode *> getChildren() const override;

  void removeChildren() override;

  AbstractLiteral *clone() const override = 0;

  iterator begin() override;
  const_iterator begin() const override;
  iterator end() override;
  const_iterator end() const override;

};

inline std::ostream &operator<<(std::ostream &os, const std::vector<AbstractLiteral *> &v) {
  os << "[";
  for (auto lit : v) { os << " " << *lit; }
  os << " ]" << std::endl;
  return os;
}

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTLITERAL_H_

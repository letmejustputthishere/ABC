#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_LITERALINT_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_LITERALINT_H_

#include <string>
#include <ostream>
#include <map>
#include "AbstractLiteral.h"

template<typename T>
class Matrix;
class AbstractMatrix;

class LiteralInt : public AbstractLiteral {
 public:
  explicit LiteralInt(AbstractMatrix *am);

  explicit LiteralInt(Matrix<AbstractExpression *> *am);

  explicit LiteralInt(Matrix<int> *inputMatrix);

  explicit LiteralInt(int value);

  LiteralInt();

  ~LiteralInt() override;

  LiteralInt *clone() const override;

  [[nodiscard]] int getValue() const;

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  bool operator==(const LiteralInt &rhs) const;

  bool operator!=(const LiteralInt &rhs) const;

  bool supportsDatatype(const Datatype &datatype) const override;

  void addLiteralValue(std::string identifier, std::unordered_map<std::string, const AbstractLiteral *> &paramsMap) override;

  void setRandomValue(RandLiteralGen &rlg) override;

  void setValue(int newValue);

  [[nodiscard]] std::string toString(bool printChildren) const override;

  void print(std::ostream &str) const override;

  bool isEqual(AbstractExpression *other) override;

  bool isNull() override;

  [[nodiscard]] AbstractMatrix *getMatrix() const override;

  void setMatrix(AbstractMatrix *newValue) override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_LITERALINT_H_

#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOR_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOR_H_

#include <utility>
#include <variant>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include "ast_opt/visitor/Visitor.h"
#include "ast_opt/ast/AbstractNode.h"
#include "ast_opt/ast/LiteralString.h"
#include "ast_opt/ast/LiteralBool.h"
#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/OpSymbEnum.h"
#include "ast_opt/ast/LiteralFloat.h"

class Operator : public AbstractNode {
 private:
  std::string operatorString;
  OpSymbolVariant operatorSymbol;

 public:
  explicit Operator(LogCompOp op);

  explicit Operator(ArithmeticOp op);

  explicit Operator(UnaryOp op);

  explicit Operator(OpSymbolVariant op);

  [[nodiscard]] const std::string &getOperatorString() const;

  [[nodiscard]] const OpSymbolVariant &getOperatorSymbol() const;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  bool isUndefined();

  bool operator==(const Operator &rhs) const;

  bool operator!=(const Operator &rhs) const;

  [[nodiscard]] bool equals(OpSymbolVariant op) const;

  AbstractLiteral *applyOperator(const AbstractLiteral *lhs, const AbstractLiteral *rhs);

  AbstractLiteral *applyOperator(const AbstractLiteral *rhs);

  template<typename A>
  AbstractLiteral *applyOperator(const A *lhs, const AbstractLiteral *rhs);

  AbstractLiteral *applyOperator(const LiteralInt *lhs, const LiteralInt *rhs);

  AbstractLiteral *applyOperator(const LiteralBool *lhs, const LiteralBool *rhs);

  AbstractLiteral *applyOperator(const LiteralInt *lhs, const LiteralBool *rhs);

  AbstractLiteral *applyOperator(const LiteralBool *lhs, const LiteralInt *rhs);

  AbstractLiteral *applyOperator(const LiteralString *lhs, const LiteralString *rhs);

  static AbstractLiteral *applyOperator(const LiteralBool *lhs, const LiteralString *rhs);

  static AbstractLiteral *applyOperator(const LiteralString *lhs, const LiteralBool *rhs);

  static AbstractLiteral *applyOperator(const LiteralInt *lhs, const LiteralString *rhs);

  static AbstractLiteral *applyOperator(const LiteralString *lhs, const LiteralInt *rhs);

  AbstractLiteral *applyOperator(const LiteralInt *rhs);

  AbstractLiteral *applyOperator(const LiteralBool *rhs);

  AbstractLiteral *applyOperator(const LiteralString *rhs);

  AbstractLiteral *applyOperator(const LiteralFloat *lhs, const LiteralFloat *rhs);

  AbstractLiteral *applyOperator(const LiteralFloat *lhs, const LiteralInt *rhs);

  AbstractLiteral *applyOperator(const LiteralInt *lhs, const LiteralFloat *rhs);

  AbstractLiteral *applyOperator(const LiteralFloat *lhs, const LiteralBool *rhs);

  AbstractLiteral *applyOperator(const LiteralBool *lhs, const LiteralFloat *rhs);

  static AbstractLiteral *applyOperator(const LiteralFloat *lhs, const LiteralString *rhs);

  static AbstractLiteral *applyOperator(const LiteralString *lhs, const LiteralFloat *rhs);

  AbstractLiteral *applyOperator(const LiteralFloat *rhs);

  [[nodiscard]] std::string toString(bool printChildren) const override;

  ~Operator() override;

  Operator *clone() const override;

  [[nodiscard]] bool equals(ArithmeticOp op) const;

  [[nodiscard]] bool equals(LogCompOp op) const;

  [[nodiscard]] bool equals(UnaryOp op) const;

  [[nodiscard]] bool isArithmeticOp() const;

  [[nodiscard]] bool isLogCompOp() const;

  [[nodiscard]] bool isUnaryOp() const;

  template<class BinaryOperation, class T>
  T accumulate(BinaryOperation op, std::vector<T> operands) {
    if (operands.empty()) throw std::out_of_range("acc requires at least one operand!");

    auto it = operands.begin();
    T result = *it;
    for (++it; it!=operands.end(); ++it) {
      result = op(result, *it);
    }
    return result;
  }

  template<class BinaryOperation, class T>
  bool applyPairwise(BinaryOperation op, std::vector<T> operands) {
    if (operands.size() < 2) throw std::out_of_range("appPairwise requires at least two operands!");

    bool result = true;
    for (auto it = operands.begin(); it!=operands.end() - 1; ++it) {
      result = result && op(*it, *(it + 1));
    }
    return result;
  }

  template<typename abstractType, typename primitiveType>
  std::vector<primitiveType> convert(std::vector<const AbstractLiteral *> operands) {
    std::vector<primitiveType> vec;
    std::transform(operands.begin(), operands.end(), std::back_inserter(vec),
                   [](const AbstractLiteral *lit) { return dynamic_cast<const abstractType*>(lit)->getValue(); });
    return vec;
  }

  AbstractLiteral *applyOperator(std::vector<const AbstractLiteral *> operands);

  AbstractLiteral *applyOperator(std::vector<int> operands);

  AbstractLiteral *applyOperator(std::vector<float> operands);

  AbstractLiteral *applyOperator(std::vector<bool> operands);

  AbstractLiteral *applyOperator(std::vector<std::string> operands);

  [[nodiscard]] bool isCommutative();

  [[nodiscard]] bool isLeftAssociative();

  /// An operator is considered as non-partial evaluable if its not possible to apply the operator only on a few
  /// operands and then store the intermediate result to continue evaluation after more operands are known.
  /// For example, 7 + a + 21 + 9 is partially evaluable to a + 37.
  /// But the expression 32 < 193 < a < 12 would partially evaluate 32 < 193 = true and then lead to true < a < 12 which
  /// is obviously not correct. Because of that we require that all operands are known for these kind of operators.
  [[nodiscard]] bool supportsPartialEvaluation();
  const std::vector<AbstractNode *> &getChildren() const override;
  void removeChildren() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_OPERATOR_H_

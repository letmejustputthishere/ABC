#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/ArithmeticExpr.h"
#include "gtest/gtest.h"

int getLiteralIntValue(AbstractExpression *aaexpr) {
  return dynamic_cast<LiteralInt *>(aaexpr)->getValue();
}

// TODO(pjattke): create tests for all possible combinations of operators and operands
// => 4 operand Types * 2 combinations (lhs or rhs) * 14 binary operators + 4 unary operators
// = 116 Tests. Generate these tests somehow automatically?
// TODO(pjattke): after having tests, improve evaluation mechanism of ArithmeticExpr

TEST(ArithmeticExprTest, aexp3Add11) { /* NOLINT */
  int valLhs = 3, valRhs = 11;
  auto *b = new ArithmeticExpr(valLhs, ArithmeticOp::ADDITION, valRhs);
  EXPECT_EQ(getLiteralIntValue(b->getLeft()), valLhs);
  EXPECT_EQ(getLiteralIntValue(b->getRight()), valRhs);
  EXPECT_EQ(b->getOperator()->getOperatorString(), "add");
}

TEST(ArithmeticExprTest, aexp2Mult9Add3) { /* NOLINT */
  int valLeft = 2, valMid = 9, valRight = 3;
  auto *b = new ArithmeticExpr(valLeft, ArithmeticOp::MULTIPLICATION,
                               new ArithmeticExpr(valMid, ArithmeticOp::ADDITION, valRight));
  // lhs: outer Aexp
  EXPECT_EQ(getLiteralIntValue(b->getLeft()), valLeft);
  EXPECT_EQ(b->getOperator()->getOperatorString(), OpSymb::getTextRepr(ArithmeticOp::MULTIPLICATION));

  // rhs: inner Aexp
  auto *r = dynamic_cast<ArithmeticExpr *>(b->getRight());
  EXPECT_EQ(getLiteralIntValue(r->getLeft()), valMid);
  EXPECT_EQ(getLiteralIntValue(r->getRight()), valRight);
  EXPECT_EQ(r->getOperator()->getOperatorString(), OpSymb::getTextRepr(ArithmeticOp::ADDITION));
}


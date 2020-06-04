#include "gtest/gtest.h"
#include "ast_opt/utilities/DotPrinter.h"
#include "ast_opt/utilities/NodeUtils.h"
#include "ast_opt/ast/AbstractNode.h"
#include "ast_opt/ast/OpSymbEnum.h"
#include "ast_opt/ast/Variable.h"
#include "ast_opt/ast/ArithmeticExpr.h"

TEST(NodeUtilsTest, rewriteMultiInputGateToBinaryGatesChain_emptyInputs) { /* NOLINT */
  std::vector<AbstractNode *> inputs{};
  ASSERT_THROW(rewriteMultiInputGateToBinaryGatesChain(inputs, LogCompOp::LOGICAL_AND), std::invalid_argument);
}

TEST(NodeUtilsTest, rewriteMultiInputGateToBinaryGatesChain_oneInputAND) { /* NOLINT */
  std::vector<AbstractNode *> inputs{new Variable("alpha")};
  LogCompOp gateType = LogCompOp::LOGICAL_AND;
  auto result = rewriteMultiInputGateToBinaryGatesChain(inputs, gateType);

// create new AST for evaluation
  Ast ast(result.back());

// if alpha = false => true AND alpha = false
  auto resultEval = ast.evaluateCircuit(
      {{"alpha", new LiteralBool(false)}}, false).front();
  ASSERT_EQ(*resultEval, LiteralBool(false));

// if alpha = true => true AND alpha = false
  resultEval = ast.evaluateCircuit(
      {{"alpha", new LiteralBool(true)}}, false).front();
  ASSERT_EQ(*resultEval, LiteralBool(true));
}

TEST(NodeUtilsTest, rewriteMultiInputGateToBinaryGatesChain_oneInputXOR) { /* NOLINT */
  std::vector<AbstractNode *> inputs{new Variable("alpha")};
  LogCompOp gateType = LogCompOp::LOGICAL_XOR;
  auto result = rewriteMultiInputGateToBinaryGatesChain(inputs, gateType);

// create new AST for evaluation
  Ast ast(result.back());

// if alpha = false => false XOR alpha = false
  auto resultEval = ast.evaluateCircuit(
      {{"alpha", new LiteralBool(false)}}, false).front();
  ASSERT_EQ(*resultEval, LiteralBool(false));

// if alpha = true => false XOR true = true
  resultEval = ast.evaluateCircuit(
      {{"alpha", new LiteralBool(true)}}, false).front();
  ASSERT_EQ(*resultEval, LiteralBool(true));
}

TEST(NodeUtilsTest, rewriteMultiInputGateToBinaryGatesChain_oneInputUnsupportedException) { /* NOLINT */
  std::vector<AbstractNode *> inputs{new Variable("alpha")};
  LogCompOp gateType = LogCompOp::GREATER;
  ASSERT_THROW(rewriteMultiInputGateToBinaryGatesChain(inputs, gateType),
               std::logic_error);
}

TEST(NodeUtilsTest, rewriteMultiInputGateToBinaryGatesChain_multipleInputs) { /* NOLINT */
  std::vector<AbstractNode *> inputs{new Variable("alpha"),
                                     new Variable("beta"),
                                     new Variable("gamma"),
                                     new Variable("delta")};
  LogCompOp gateType = LogCompOp::LOGICAL_XOR;
  auto result = rewriteMultiInputGateToBinaryGatesChain(inputs, gateType);

// create new AST for evaluation
  Ast ast(result.back());

// if all are false => evaluate to false
  auto resultEval = ast.evaluateCircuit(
      {{"alpha", new LiteralBool(false)},
       {"beta", new LiteralBool(false)},
       {"gamma", new LiteralBool(false)},
       {"delta", new LiteralBool(false)}},
      false).front();
  ASSERT_EQ(*resultEval, LiteralBool(false));

// if exactly one is true => evaluate to true
  resultEval = ast.evaluateCircuit(
      {{"alpha", new LiteralBool(false)},
       {"beta", new LiteralBool(false)},
       {"gamma", new LiteralBool(true)},
       {"delta", new LiteralBool(false)}},
      false).front();
  ASSERT_EQ(*resultEval, LiteralBool(true));

// if multiple are true => evaluate to false
  resultEval = ast.evaluateCircuit(
      {{"alpha", new LiteralBool(true)},
       {"beta", new LiteralBool(false)},
       {"gamma", new LiteralBool(true)},
       {"delta", new LiteralBool(false)}},
      false).front();
  ASSERT_EQ(*resultEval, LiteralBool(false));

// if all are true => evaluate to false
  resultEval = ast.evaluateCircuit(
      {{"alpha", new LiteralBool(true)},
       {"beta", new LiteralBool(true)},
       {"gamma", new LiteralBool(true)},
       {"delta", new LiteralBool(true)}},
      false).front();
  ASSERT_EQ(*resultEval, LiteralBool(false));
}

TEST(NodeUtilsTest, createMultDepthBalancedTreeFromInputs_simpleTestArithmeticalExpr) {  /* NOLINT */
  // create a map of depths (would usually be computed by MultiplicativeDepthCalculator)
  std::unordered_map<std::string, int> multiplicativeDepths;

  // create some nodes and assign a multiplicative depth to variables
  AbstractExpression *literalInt5 = new LiteralInt(5);  // implicit: depth 0
  AbstractExpression *literalInt9 = new LiteralInt(9);  // implicit: depth 0
  AbstractExpression *literalInt21 = new LiteralInt(21);
  AbstractExpression *variableX = new Variable("x");
  multiplicativeDepths[variableX->getUniqueNodeId()] = 8;
  AbstractExpression *variableY = new Variable("y");
  multiplicativeDepths[variableY->getUniqueNodeId()] = 2;

  // create the multiplicative depth-balanced tree
  auto opSymbol = ArithmeticOp::MULTIPLICATION;
  auto generatedAst = createMultDepthBalancedTreeFromInputs(
      {literalInt5, literalInt9, variableX, variableY, literalInt21}, opSymbol, multiplicativeDepths);

  // check that the resulting tree looks like the expected one
  auto expectedAst =
      new ArithmeticExpr(
          new ArithmeticExpr(
              new ArithmeticExpr(literalInt21, opSymbol, literalInt9),
              opSymbol,
              new ArithmeticExpr(literalInt5, opSymbol, variableY)),
          opSymbol,
          variableX);
  ASSERT_TRUE(generatedAst->castTo<AbstractBinaryExpr>()->isEqual(expectedAst));
}

TEST(NodeUtilsTest, createMultDepthBalancedTreeFromInputs_complexTest) {  /* NOLINT */
  // create a map of depths (would usually be computed by MultiplicativeDepthCalculator)
  std::unordered_map<std::string, int> multiplicativeDepths;

  // create some nodes and assign a multiplicative depth to variables
  AbstractExpression *literalInt5 = new LiteralInt(5);  // implicit: depth 0
  AbstractExpression *literalInt9 = new LiteralInt(9);  // implicit: depth 0
  AbstractExpression *variableW = new Variable("w");
  multiplicativeDepths[variableW->getUniqueNodeId()] = 0;
  AbstractExpression *variableX = new Variable("x");
  multiplicativeDepths[variableX->getUniqueNodeId()] = 8;
  AbstractExpression *variableY = new Variable("y");
  multiplicativeDepths[variableY->getUniqueNodeId()] = 2;
  AbstractExpression *variableZ = new Variable("z");
  multiplicativeDepths[variableZ->getUniqueNodeId()] = 12;

  // create the multiplicative depth-balanced tree (intentionally, insert in random order)
  auto opSymbol = ArithmeticOp::ADDITION;
  auto generatedAst = createMultDepthBalancedTreeFromInputs(
      {variableZ, literalInt5, variableY, literalInt9, variableW, variableX},
      opSymbol,
      multiplicativeDepths);

  // heck that the resulting tree looks like the expected one
  auto expectedAst =
      new ArithmeticExpr(
          new ArithmeticExpr(
              new ArithmeticExpr(variableW, opSymbol, literalInt9),
              opSymbol,
              new ArithmeticExpr(literalInt5, opSymbol, variableY)),
          opSymbol,
          new ArithmeticExpr(variableX, opSymbol, variableZ));
  ASSERT_TRUE(generatedAst->castTo<AbstractBinaryExpr>()->isEqual(expectedAst));
}

TEST(NodeUtilsTest, createMultDepthBalancedTreeFromInputs_simpleTestLogicalExpr) {  /* NOLINT */
  // create a map of depths (would usually be computed by MultiplicativeDepthCalculator)
  std::unordered_map<std::string, int> multiplicativeDepths;

  // create some nodes and assign a multiplicative depth to variables
  AbstractExpression *literalInt5 = new LiteralBool(true);  // implicit: depth 0
  AbstractExpression *literalInt9 = new LiteralBool(false);  // implicit: depth 0
  AbstractExpression *literalInt21 = new LiteralBool(false);
  AbstractExpression *variableX = new Variable("x");
  multiplicativeDepths[variableX->getUniqueNodeId()] = 8;
  AbstractExpression *variableY = new Variable("y");
  multiplicativeDepths[variableY->getUniqueNodeId()] = 2;

  auto const opSymb(LogCompOp::LOGICAL_AND);

  // create the multiplicative depth-balanced tree
  auto generatedAst = createMultDepthBalancedTreeFromInputs(
      {literalInt5, literalInt9, variableX, variableY, literalInt21}, opSymb, multiplicativeDepths);

  // check that the resulting tree looks like the expected one
  auto expectedAst =
      new LogicalExpr(
          new LogicalExpr(
              new LogicalExpr(literalInt21, opSymb, literalInt9),
              opSymb,
              new LogicalExpr(literalInt5, opSymb, variableY)),
          opSymb,
          variableX);
  ASSERT_TRUE(generatedAst->castTo<AbstractBinaryExpr>()->isEqual(expectedAst));
}

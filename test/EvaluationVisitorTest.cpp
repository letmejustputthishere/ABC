#include "gtest/gtest.h"
#include "AstTestingGenerator.h"
#include "ast_opt/visitor/PrintVisitor.h"
#include "ast_opt/ast/Ast.h"
#include "ast_opt/ast/Operator.h"
#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/LiteralBool.h"
#include "ast_opt/ast/LiteralString.h"
#include "ast_opt/ast/FunctionParameter.h"
#include "ast_opt/ast/Variable.h"
#include "ast_opt/ast/ArithmeticExpr.h"
#include "ast_opt/ast/Return.h"
#include "ast_opt/ast/Function.h"
#include "ast_opt/ast/Call.h"
#include "ast_opt/ast/Rotate.h"
#include "ast_opt/ast/VarAssignm.h"
#include "ast_opt/ast/MatrixAssignm.h"
#include "ast_opt/ast/MatrixElementRef.h"

TEST(EvaluationVisitorTests, simpleAstEvaluation1) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(7, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"width", new LiteralInt(31)},
      {"length", new LiteralInt(87)},
      {"depth", new LiteralInt(771)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(693'129));
}

TEST(EvaluationVisitorTests, simpleAstEvaluationIfThenBranch) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(8, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"inputA", new LiteralInt(852)},
      {"inputB", new LiteralInt(7)},
      {"takeIf", new LiteralBool(true)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(0));
}

TEST(EvaluationVisitorTests, simpleAstEvaluationIfElseBranch) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(8, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"inputA", new LiteralInt(852)},
      {"inputB", new LiteralInt(7)},
      {"takeIf", new LiteralBool(false)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(5943));
}

TEST(EvaluationVisitorTests, simpleAstEvaluationUnaryLogExpr1) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(9, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"inputA", new LiteralInt(11)},
      {"inputB", new LiteralInt(-500)},
      {"strong", new LiteralBool(true)},
      {"negate", new LiteralBool(true)}
  };
  auto result = dynamic_cast<const LiteralBool *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralBool(true));
}

TEST(EvaluationVisitorTests, simpleAstEvaluationUnaryLogExpr2) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(9, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"inputA", new LiteralInt(11)},
      {"inputB", new LiteralInt(12)},
      {"strong", new LiteralBool(true)},
      {"negate", new LiteralBool(true)}
  };
  auto result = dynamic_cast<const LiteralBool *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralBool(false));
}

TEST(EvaluationVisitorTests, simpleAstEvaluationUnaryLogExpr3) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(9, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"inputA", new LiteralInt(11)},
      {"inputB", new LiteralInt(461)},
      {"strong", new LiteralBool(true)},
      {"negate", new LiteralBool(false)}
  };
  auto result = dynamic_cast<const LiteralBool *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralBool(true));
}

TEST(EvaluationVisitorTests, simpleAstEvaluationStringConcat) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(10, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"strA", new LiteralString("hello ")},
      {"strB", new LiteralString("world!")}
  };
  auto result = dynamic_cast<const LiteralString *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralString("hello world!"));
}

TEST(EvaluationVisitorTests, complexAstEvaluationWhileNotExecuted) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(11, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"encryptedA", new LiteralInt(1)},
      {"encryptedB", new LiteralInt(7)},
      {"randInt", new LiteralInt(4)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(0));
}

TEST(EvaluationVisitorTests, complexAstEvaluationWhileExecutedThreeTimes) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(11, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"encryptedA", new LiteralInt(1'182)},
      {"encryptedB", new LiteralInt(7)},
      {"randInt", new LiteralInt(3)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(21));
}

TEST(EvaluationVisitorTests, complexAstEvaluationWithNestedFunctionCall_LiteralParameterValue) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(12, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params;
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(1'056));
}

TEST(EvaluationVisitorTests, complexAstEvaluationWithNestedFunctionCall_AexpParameterValue) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(13, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params;
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(7'168));
}

TEST(EvaluationVisitorTests, astIncludingForLoop) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(23, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params({{"inputA", new LiteralInt(5)}});
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(30));
}

TEST(EvaluationVisitorTests, astLiteralRotation) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(24, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<int> *>(result->getMatrix());
  EXPECT_EQ(m(0, 0), 3);
  EXPECT_EQ(m(0, 1), 1);
  EXPECT_EQ(m(0, 2), 7);
}

TEST(EvaluationVisitorTests, astRotateAndSumVector) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(25, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<int> *>(result->getMatrix());
  EXPECT_EQ(m.values, std::vector<std::vector<int>>({{11, 11, 11}}));
}

TEST(EvaluationVisitorTests, astTransposeMatrix) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(26, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<int> *>(result->getMatrix());
  EXPECT_EQ(m.values, std::vector<std::vector<int>>({{11, 4, 2}, {2, 2, 1}, {3, 3, 3}}));
}

TEST(EvaluationVisitorTests, astGetSingleMatrixElement) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(27, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<int> *>(result->getMatrix());
  EXPECT_EQ(m.values, std::vector<std::vector<int>>({{27}}));
}

TEST(EvaluationVisitorTests, astCombineMatrices_LiteralInt) { /* NOLINT */
  //  -- input --
  // extractArbitraryMatrixElements {
  //   int M = [ 14 27 32 ];
  //   int B = [ 19 21 38 ];
  //   return [ M[0][1];      // ret0
  //            B[0][0];      // ret1
  //            B[0][2] ];    // ret2
  // }
  //  -- expected --
  //  result: [14 19 38];
  Ast ast;
  AstTestingGenerator::generateAst(28, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<int> *>(result->getMatrix());
  EXPECT_EQ(m.values, std::vector<std::vector<int>>({{27, 19, 38}}));
}

TEST(EvaluationVisitorTests, astCombineMatrices_LiteralFloat) { /* NOLINT */
  //  -- expected --
  //  result: [1.4 1.9 3.8];
  Ast ast;
  AstTestingGenerator::generateAst(29, ast);
  auto result = dynamic_cast<const LiteralFloat *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<float> *>(result->getMatrix());
  EXPECT_EQ(m.values, std::vector<std::vector<float>>({{2.7f, 1.9f, 3.8f}}));
}

TEST(EvaluationVisitorTests, astCombineMatrices_LiteralBool) { /* NOLINT */
  //  -- expected --
  //  result: [true false true];
  Ast ast;
  AstTestingGenerator::generateAst(30, ast);
  auto result = dynamic_cast<const LiteralBool *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<bool> *>(result->getMatrix());
  EXPECT_EQ(m.values, std::vector<std::vector<bool>>({{true, false, true}}));
}

TEST(EvaluationVisitorTests, astCombineMatrices_LiteralString) { /* NOLINT */
  //  -- expected --
  //  result: ["alpha" "beta" "gamma"];
  Ast ast;
  AstTestingGenerator::generateAst(31, ast);
  auto result = dynamic_cast<const LiteralString *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<std::string> *>(result->getMatrix());
  EXPECT_EQ(m.values, std::vector<std::vector<std::string>>({{"alpha", "beta", "gamma"}}));
}

TEST(EvaluationVisitorTests, astCrossProduct) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(32, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  auto m = *dynamic_cast<const Matrix<int> *>(result->getMatrix());
  EXPECT_EQ(m.values, std::vector<std::vector<int>>({{354, 76, -219}}));
}

TEST(EvaluationVisitorTests, astOperatorExprSimple) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(37, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"a", new LiteralInt(63)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(66'402));
}

TEST(EvaluationVisitorTests, astOperatorNestedOperands) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(47, ast);
  auto result = dynamic_cast<const LiteralBool *>(ast.evaluateAst({}, false).front());
  ASSERT_EQ(*result, *new LiteralBool(false));
}

TEST(EvaluationVisitorTests, complexRotationExample) {  /* NOLINT */
  // rotate([1; 2; 3; 4], 2) + [1; 2; 3; 4]
  // = [3; 4; 1; 2]          + [1; 2; 3; 4]
  // = [4; 6; 4; 6]
  auto vec2 = new LiteralInt(new Matrix<int>({{1}, {2}, {3}, {4}}));
  auto vec2RotationFactor = new LiteralInt(2);
  auto rotateVec2 = new Rotate(vec2, vec2RotationFactor);
  auto vec1 = new LiteralInt(new Matrix<int>({{1}, {2}, {3}, {4}}));
  auto addVec1Vec2 = new ArithmeticExpr(vec1, ADDITION, rotateVec2);

  // rotate(rotate([1; 2; 3; 4], 2) + [1; 2; 3; 4], 1)
  // = rotate([3; 4; 1; 2]          + [1; 2; 3; 4], 1)
  // = rotate([4; 6; 4; 6], 1)
  // = [6; 4; 6; 4]
  auto vec4 = new LiteralInt(new Matrix<int>({{1}, {2}, {3}, {4}}));
  auto vec4RotationFactor = new LiteralInt(2);
  auto rotateVec4 = new Rotate(vec4, vec4RotationFactor);
  auto vec3 = new LiteralInt(new Matrix<int>({{1}, {2}, {3}, {4}}));
  auto addVec3Vec4 = new ArithmeticExpr(vec3, ADDITION, rotateVec4);
  auto vec3AddedVec4RotationFactor = new LiteralInt(1);
  auto rotateVec3AddedVec4 = new Rotate(addVec3Vec4, vec3AddedVec4RotationFactor);

  // [4; 6; 4; 6] + [6; 4; 6; 4]; = [10; 10; 10; 10]
  auto finalResult = new ArithmeticExpr(addVec1Vec2, ADDITION, rotateVec3AddedVec4);

  // return [4; 6; 4; 6] + [6; 4; 6; 4];
  auto ret = new Return(finalResult);

  // create function signature and append the Return statement to the function's body
  auto function = new Function("rotateAddMatrices");
  function->addStatement(ret);
  Ast ast;
  ast.setRootNode(function);

  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  EXPECT_EQ(*result, LiteralInt(new Matrix<int>({{10}, {10}, {10}, {10}})));
}

TEST(EvaluationVisitorTests, astMatrixAssignmentFromLiteralInt) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(50, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  ASSERT_EQ(*result, *new LiteralInt(new Matrix<int>({{11, 27, 32}})));
}

TEST(EvaluationVisitorTests, astMatrixPermutation) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(51, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  ASSERT_EQ(*result, *new LiteralInt(new Matrix<int>({{32, 27, 14}})));
}

TEST(EvaluationVisitorTests, astGetMatrixSizeKnownMatrix) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(52, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  std::vector<std::vector<int>> v = {{44}};
  ASSERT_EQ(*result, *new LiteralInt(new Matrix<int>(v)));
}

TEST(EvaluationVisitorTests, astGetMatrixSizeAbstractMatrix) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(53, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"factor", new LiteralInt(42)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(new Matrix<int>({{1, 5, 0}})));
}

TEST(EvaluationVisitorTests, astGetMatrixSizeUnknownMatrix) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(54, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"inputMatrix", new LiteralInt(new Matrix<int>({{33, 111, 3, 994}}))},
      {"dimension", new LiteralInt(1)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  ASSERT_EQ(*result, *new LiteralInt(4));
}

TEST(EvaluationVisitorTests, astMatrixAssignmAndGetMatrixSize) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(55, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  ASSERT_EQ(*result, *new LiteralInt(new Matrix<AbstractExpr *>(
      {{new LiteralInt(0), new LiteralInt(0), new LiteralInt(0)},
       {new LiteralInt(0), new LiteralInt(1), new LiteralInt(2)},
       {new LiteralInt(0), new LiteralInt(2), new LiteralInt(4)}})));
}

TEST(EvaluationVisitorTests, astMatrixAssignmentUnknownThenKnown) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(56, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"k", new LiteralInt(2)},
      {"a", new LiteralInt(19)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  auto expectedResult = new LiteralInt(new Matrix<AbstractExpr *>({{new LiteralInt(40)},
                                                                   {nullptr},
                                                                   {new LiteralInt(4)}}));
  ASSERT_EQ(*result, *expectedResult);
}

TEST(EvaluationVisitorTests, astMatrixAssignmentKnownThenUnknown) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(57, ast);
  std::unordered_map<std::string, const AbstractLiteral *> params = {
      {"k", new LiteralInt(5)}
  };
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst(params, false).front());
  auto expectedResult = new LiteralInt(new Matrix<AbstractExpr *>({{new LiteralInt(21),
                                                                    nullptr,
                                                                    nullptr,
                                                                    nullptr,
                                                                    nullptr,
                                                                    new LiteralInt(4)}}));
  ASSERT_EQ(*result, *expectedResult);
}

TEST(EvaluationVisitorTests, astFullAssignmentToMatrix) { /* NOLINT */
  Ast ast;
  AstTestingGenerator::generateAst(58, ast);
  auto result = dynamic_cast<const LiteralInt *>(ast.evaluateAst({}, false).front());
  auto expectedResult = new LiteralInt(new Matrix<int>({{11, 1, 1}, {3, 2, 2}}));
  ASSERT_EQ(*result, *expectedResult);
}

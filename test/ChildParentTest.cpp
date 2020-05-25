#include "gtest/gtest.h"
#include "ast_opt/ast/ArithmeticExpr.h"
#include "ast_opt/ast/Variable.h"
#include "ast_opt/ast/Return.h"
#include "ast_opt/ast/Function.h"
#include "ast_opt/ast/LiteralFloat.h"
#include "ast_opt/ast/LogicalExpr.h"
#include "ast_opt/ast/UnaryExpr.h"
#include "ast_opt/ast/While.h"
#include "ast_opt/ast/Block.h"
#include "ast_opt/ast/Call.h"
#include "ast_opt/ast/CallExternal.h"
#include "ast_opt/ast/If.h"
#include "ast_opt/ast/VarAssignm.h"
#include "ast_opt/ast/VarDecl.h"
#include "ast_opt/ast/For.h"
#include "ast_opt/ast/OperatorExpr.h"

class ArithmeticExprFixture : public ::testing::Test {
 protected:
  LiteralInt *left;
  LiteralInt *otherLeft;
  LiteralFloat *right;
  ArithmeticOp opSymb;
  Operator *operatorAdd;

  ArithmeticExprFixture() {
    left = new LiteralInt(3);
    otherLeft = new LiteralInt(42);
    right = new LiteralFloat(2.0);
    opSymb = ArithmeticOp::ADDITION;
    operatorAdd = new Operator(opSymb);
  }
};

TEST_F(ArithmeticExprFixture, ArithmeticExprStandardConstructor) {  /* NOLINT */
  auto arithmeticExpr = new ArithmeticExpr(left, opSymb, right);

  // children
  ASSERT_EQ(arithmeticExpr->getChildren().size(), 3);
  ASSERT_EQ(arithmeticExpr->getLeft(), left);
  ASSERT_TRUE(arithmeticExpr->getOperator()->equals(opSymb));
  ASSERT_EQ(arithmeticExpr->getRight(), right);

  // parents
  ASSERT_EQ(arithmeticExpr->getParent(), nullptr);
  ASSERT_EQ(arithmeticExpr->getLeft()->getParent()!=nullptr, 1);
  ASSERT_TRUE(arithmeticExpr->getLeft()->hasParent(arithmeticExpr));
  ASSERT_EQ(arithmeticExpr->getOperator()->getParent()!=nullptr, 1);
  ASSERT_TRUE(arithmeticExpr->getOperator()->hasParent(arithmeticExpr));
  ASSERT_EQ(arithmeticExpr->getRight()->getParent()!=nullptr, 1);
  ASSERT_TRUE(arithmeticExpr->getRight()->hasParent(arithmeticExpr));
}

TEST_F(ArithmeticExprFixture, ArithmeticExprEmptyConstructor) {  /* NOLINT */
  ArithmeticExpr arithmeticExpr;
  ASSERT_EQ(arithmeticExpr.getChildren().size(), 3);
  ASSERT_EQ(arithmeticExpr.countChildrenNonNull(), 0);
  ASSERT_EQ(arithmeticExpr.getParent(), nullptr);
}

TEST_F(ArithmeticExprFixture, ArithmeticExprOperatorOnlyConstructor) {  /* NOLINT */
  auto arithmeticExpr = new ArithmeticExpr(opSymb);

  // children
  ASSERT_EQ(arithmeticExpr->getChildren().size(), 3);
  ASSERT_EQ(arithmeticExpr->countChildrenNonNull(), 1);
  ASSERT_EQ(arithmeticExpr->getLeft(), nullptr);
  ASSERT_TRUE(arithmeticExpr->getOperator()->equals(opSymb));
  ASSERT_EQ(arithmeticExpr->getRight(), nullptr);

  // parents
  ASSERT_EQ(arithmeticExpr->getParent(), nullptr);
  ASSERT_EQ(arithmeticExpr->getOperator()->getParent()!=nullptr, 1);
  ASSERT_TRUE(arithmeticExpr->getOperator()->hasParent(arithmeticExpr));
}

TEST_F(ArithmeticExprFixture, ArithmeticExprAddChildException_NoEmptyChildSpotAvailable) {  /* NOLINT */
  auto arithmeticExpr = new ArithmeticExpr(left, opSymb, right);
  EXPECT_THROW(arithmeticExpr->addChild(new LiteralInt(3), false),
               std::logic_error);
}

TEST_F(ArithmeticExprFixture, ArithmeticExprAddChildException_TooManyChildrenAdded) {  /* NOLINT */
  auto arithmeticExpr = new ArithmeticExpr(left, opSymb, right);
  EXPECT_THROW(arithmeticExpr->addChildren({{left, otherLeft, new Operator(opSymb), right}}),
               std::invalid_argument);
}

TEST_F(ArithmeticExprFixture, ArithmeticExprAddChildSuccess) {  /* NOLINT */
  auto arithmeticExpr = new ArithmeticExpr();
  arithmeticExpr->setAttributes(nullptr, operatorAdd, right);
  auto newLeft = new LiteralInt(3);
  arithmeticExpr->addChild(newLeft, true);

  // children
  EXPECT_EQ(arithmeticExpr->getChildren().size(), 3);
  EXPECT_EQ(arithmeticExpr->getLeft(), newLeft);
  EXPECT_EQ(arithmeticExpr->getOperator(), operatorAdd);
  EXPECT_TRUE(arithmeticExpr->getOperator()->equals(opSymb));
  EXPECT_EQ(arithmeticExpr->getRight(), right);

  // parents
  EXPECT_EQ(newLeft->getParent()!=nullptr, 1);
  EXPECT_EQ(newLeft->getParent(), arithmeticExpr);
  EXPECT_EQ(operatorAdd->getParent()!=nullptr, 1);
  EXPECT_EQ(operatorAdd->getParent(), arithmeticExpr);
  EXPECT_EQ(right->getParent()!=nullptr, 1);
  EXPECT_EQ(right->getParent(), arithmeticExpr);
}

TEST(ChildParentTests, Block) {  /* NOLINT */
  auto blockStatement =
      new Block(new VarAssignm("varX", new LiteralInt(22)));
  ASSERT_EQ(blockStatement->getChildren().size(), 1);
  ASSERT_EQ(blockStatement->getParent(), nullptr);
  ASSERT_EQ(blockStatement->getMaxNumberChildren(), -1);
}

TEST(ChildParentTests, Block_addAdditionalChild) {  /* NOLINT */
  auto varDecl = new VarDecl("varX", 22);
  auto blockStatement = new Block(varDecl);
  auto varAssignm = new VarAssignm("varX", new LiteralInt(531));
  blockStatement->addChild(varAssignm);

  ASSERT_EQ(blockStatement->getMaxNumberChildren(), -1);

  ASSERT_EQ(blockStatement->getChildren().size(), 2);
  ASSERT_EQ(blockStatement->getParent(), nullptr);

  EXPECT_EQ(blockStatement->getStatements().at(0)->getParent()!=nullptr, 1);
  EXPECT_EQ(blockStatement->getStatements().at(0), varDecl);
  EXPECT_EQ(blockStatement->getStatements().at(1)->getParent()!=nullptr, 1);
  EXPECT_EQ(blockStatement->getStatements().at(1), varAssignm);
}

TEST(ChildParentTests, CallStandardConstructor) {  /* NOLINT */
  auto func = new Function("computeSecretX");
  auto funcParam = new FunctionParameter(new Datatype(Types::INT), new LiteralInt(221));
  auto call = new Call({funcParam}, func);

  ASSERT_EQ(call->getMaxNumberChildren(), 2);

  // children
  ASSERT_EQ(call->getChildren().size(), 2);
  ASSERT_TRUE(call->getParameterList()!=nullptr);
  ASSERT_EQ(call->getFunc(), func);

  // parents
  ASSERT_EQ(call->getParent(), nullptr);
  ASSERT_EQ(call->getParameterList()->getParent()!=nullptr, 1);
  ASSERT_TRUE(call->getParameterList()->hasParent(call));
  ASSERT_EQ(call->getFunc()->getParent()!=nullptr, 1);
  ASSERT_TRUE(call->getFunc()->hasParent(call));
}

TEST(ChildParentTests, CallArgumentlessConstructor) {  /* NOLINT */
  auto func = new Function("computeSecretX");
  auto call = new Call(func);

  ASSERT_EQ(call->getMaxNumberChildren(), 2);

  // children
  ASSERT_EQ(call->getChildren().size(), 2);
  ASSERT_TRUE(call->getParameterList()!=nullptr);
  ASSERT_EQ(call->getFunc(), func);

  // parents
  ASSERT_EQ(call->getParent(), nullptr);
  ASSERT_EQ(call->getFunc()->getParent()!=nullptr, 1);
  ASSERT_TRUE(call->getFunc()->hasParent(call));
}

TEST(ChildParentTests, CallAddChildException_NoEmptyChildSpotAvailable) {  /* NOLINT */
  auto func = new Function("computeSecretX");
  auto arithmeticExpr = new Call(func);
  EXPECT_THROW(arithmeticExpr->addChild(new Function("y")), std::logic_error);
}

TEST(ChildParentTests, CallAddChildSuccess) {  /* NOLINT */
  auto func = new Function("computeSecretX");
  auto funcParam = new FunctionParameter(new Datatype(Types::INT), new LiteralInt(221));
  auto call = new Call({funcParam}, func);
  auto newChild = new FunctionParameter(new Datatype(Types::INT, true), new Variable("seed"));
  call->getParameterList()->addChild(newChild);

  // children
  EXPECT_EQ(call->getChildren().size(), 2);
  EXPECT_TRUE(dynamic_cast<ParameterList *>(call->getParameterList())!=nullptr);
  EXPECT_EQ(call->getParameterList()->getParameters().at(0), funcParam);
  EXPECT_EQ(call->getParameterList()->getParameters().at(1), newChild);
  EXPECT_EQ(call->getFunc(), func);

  // parents
  EXPECT_EQ(call->getParent(), nullptr);
  EXPECT_EQ(func->getParent(), call);
  auto paramList = call->getParameterList();
  EXPECT_EQ(paramList->getParent(), call);
  EXPECT_EQ(paramList->getParameters().at(0)->getParent(), paramList);
  EXPECT_EQ(paramList->getParameters().at(1)->getParent(), paramList);
}

TEST(ChildParentTests, CallExternal) {  /* NOLINT */
  auto callExternal = new CallExternal("computeSecretKeys");

  // using AbstractExpr
  ASSERT_EQ(callExternal->AbstractExpr::getChildren().size(), 0);
  ASSERT_EQ(callExternal->AbstractExpr::getParent(), nullptr);
  ASSERT_EQ(callExternal->AbstractExpr::getMaxNumberChildren(), 0);
}

class FunctionFixture : public ::testing::Test {
 protected:
  Function *funcComputeX;
  AbstractStatement *returnStatement;

  FunctionFixture() {
    funcComputeX = new Function("computeX");
    returnStatement = new Return(new LiteralBool(true));
  }
};

TEST_F(FunctionFixture, FunctionAddStatement) {  /* NOLINT */
  funcComputeX->addStatement(returnStatement);
  ASSERT_EQ(funcComputeX->getChildren().size(), 2);
  ASSERT_EQ(returnStatement->getParent()!=nullptr, 1);
}

TEST_F(FunctionFixture, FunctionSupportedInCircuitMode) {  /* NOLINT */
  // Function is circuit-compatible, i.e., supports use of child/parent relationship
  ASSERT_EQ(funcComputeX->getChildren().size(), 2);
  ASSERT_EQ(returnStatement->getParent(), nullptr);
  ASSERT_EQ(funcComputeX->getMaxNumberChildren(), 2);
}

class FunctionParameterFixture : public ::testing::Test {
 protected:
  Datatype *datatype;
  Datatype *datatype2;
  Types datatypeEnum;
  std::string datatypeAsString;
  AbstractExpr *variableThreshold;
  AbstractExpr *variableSecret;

  FunctionParameterFixture() {
    datatypeEnum = Types::INT;
    datatypeAsString = Datatype::enumToString(datatypeEnum);
    datatype = new Datatype(datatypeEnum);
    datatype2 = new Datatype(Types::FLOAT);
    variableThreshold = new Variable("threshold");
    variableSecret = new Variable("secretNumber");
  }
};

TEST_F(FunctionParameterFixture, FunctionParameterStandardConstructor) {  /* NOLINT */
  auto functionParameter = new FunctionParameter(datatypeAsString, variableThreshold);

  // children
  ASSERT_EQ(functionParameter->getChildren().size(), 2);
  ASSERT_EQ(functionParameter->getDatatype()->castTo<Datatype>()->getType(), datatypeEnum);
  ASSERT_EQ(functionParameter->getValue(), variableThreshold);

  // parents
  ASSERT_EQ(functionParameter->getParent(), nullptr);
  ASSERT_EQ(functionParameter->getDatatype()->getParent()!=nullptr, 1);
  ASSERT_TRUE(functionParameter->getDatatype()->hasParent(functionParameter));
  ASSERT_EQ(functionParameter->getValue()->getParent()!=nullptr, 1);
  ASSERT_TRUE(functionParameter->getValue()->hasParent(functionParameter));
}

TEST_F(FunctionParameterFixture, FunctionParameterAddChildExceptionDatatypeConstructor) {  /* NOLINT */
  auto functionParameter = new FunctionParameter(datatype, variableThreshold);

  // children
  ASSERT_EQ(functionParameter->getChildren().size(), 2);
  ASSERT_EQ(functionParameter->getDatatype(), datatype);
  ASSERT_EQ(functionParameter->getValue(), variableThreshold);

  // parents
  ASSERT_EQ(functionParameter->getParent(), nullptr);
  ASSERT_EQ(functionParameter->getDatatype()->getParent()!=nullptr, 1);
  ASSERT_TRUE(functionParameter->getDatatype()->hasParent(functionParameter));
  ASSERT_EQ(functionParameter->getValue()->getParent()!=nullptr, 1);
  ASSERT_TRUE(functionParameter->getValue()->hasParent(functionParameter));
}

TEST_F(FunctionParameterFixture, FunctionParameterAddChildException_NoEmptyChildSpotAvailable) {  /* NOLINT */
  auto functionParameter = new FunctionParameter(datatype, variableThreshold);
  EXPECT_THROW(functionParameter->addChild(variableSecret, false), std::logic_error);
}

TEST_F(FunctionParameterFixture, FunctionParameterAddChildException_TooManyChildrenAdded) {  /* NOLINT */
  auto functionParameter = new FunctionParameter(datatype, variableThreshold);
  EXPECT_THROW(functionParameter->addChildren({{datatype, variableSecret, variableThreshold}}),
               std::invalid_argument);
}

TEST_F(FunctionParameterFixture, FunctionParameter_AddChildSuccess) {  /* NOLINT */
  auto functionParameter = new FunctionParameter(datatype, variableThreshold);

  functionParameter->removeChild(variableThreshold, false);
  functionParameter->addChild(variableSecret, true);

  // children
  EXPECT_EQ(functionParameter->getChildren().size(), 2);
  EXPECT_EQ(functionParameter->getValue(), variableSecret);

  // parents
  EXPECT_EQ(functionParameter->getParent(), nullptr);
  EXPECT_EQ(variableSecret->getParent()!=nullptr, 1);
  EXPECT_EQ(variableSecret->getParent(), functionParameter);
  EXPECT_TRUE(variableSecret->hasParent(functionParameter));

  functionParameter->removeChild(datatype, false);
  functionParameter->addChild(datatype2, true);

  // children
  EXPECT_EQ(functionParameter->getChildren().size(), 2);
  EXPECT_EQ(functionParameter->getDatatype(), datatype2);

  // parents
  EXPECT_EQ(functionParameter->getParent(), nullptr);
  EXPECT_EQ(datatype2->getParent()!=nullptr, 1);
  EXPECT_EQ(datatype2->getParent(), functionParameter);
  EXPECT_TRUE(datatype2->hasParent(functionParameter));
}

class IfStmtFixture : public ::testing::Test {
 protected:
  AbstractExpr *condition;
  AbstractStatement *thenBranch, *elseBranch;
  IfStmtFixture() {
    condition = new LogicalExpr(new LiteralInt(33), LogCompOp::GREATER, new CallExternal("computeX"));
    thenBranch = new Block(new VarAssignm("a", new LiteralInt(22)));
    elseBranch = new Block(new VarAssignm("a", new LiteralInt(175)));
  }
};

TEST_F(IfStmtFixture, IfStmtThenOnlyConstructor) {  /* NOLINT */
  auto ifStmt = new If(condition, thenBranch);

  // children
  ASSERT_EQ(ifStmt->getChildren().size(), 3);
  ASSERT_EQ(ifStmt->getCondition(), condition);
  ASSERT_EQ(ifStmt->getThenBranch(), thenBranch);

  // parents
  ASSERT_EQ(ifStmt->getParent(), nullptr);
  ASSERT_EQ(ifStmt->getCondition()->getParent()!=nullptr, 1);
  ASSERT_TRUE(ifStmt->getCondition()->hasParent(ifStmt));
  ASSERT_EQ(ifStmt->getThenBranch()->getParent()!=nullptr, 1);
  ASSERT_TRUE(ifStmt->getThenBranch()->hasParent(ifStmt));
}

TEST_F(IfStmtFixture, IfStmtThenAndElseConstructor) {  /* NOLINT */
  auto ifStmt = new If(condition, thenBranch, elseBranch);

  // children
  ASSERT_EQ(ifStmt->getChildren().size(), 3);
  ASSERT_EQ(ifStmt->getCondition(), condition);
  ASSERT_EQ(ifStmt->getThenBranch(), thenBranch);
  ASSERT_EQ(ifStmt->getElseBranch(), elseBranch);

  // parents
  ASSERT_EQ(ifStmt->getParent(), nullptr);
  ASSERT_EQ(ifStmt->getCondition()->getParent()!=nullptr, 1);
  ASSERT_TRUE(ifStmt->getCondition()->hasParent(ifStmt));
  ASSERT_EQ(ifStmt->getThenBranch()->getParent()!=nullptr, 1);
  ASSERT_TRUE(ifStmt->getThenBranch()->hasParent(ifStmt));
  ASSERT_EQ(ifStmt->getElseBranch()->getParent()!=nullptr, 1);
  ASSERT_TRUE(ifStmt->getElseBranch()->hasParent(ifStmt));
}

TEST_F(IfStmtFixture, IfStmtAddChildException_NoEmptyChildSpotAvailable) {  /* NOLINT */
  auto ifStmt = new If(condition, thenBranch, elseBranch);
  EXPECT_THROW(ifStmt->addChild(new VarAssignm("a", new LiteralInt(22222)), false),
               std::logic_error);
}

TEST_F(IfStmtFixture, IfStmtAddChildException_TooManyChildrenAdded) {  /* NOLINT */
  auto ifStmt = new If(condition, thenBranch);
  auto newElseBranch = new Block(new VarAssignm("a", new LiteralInt(1024)));
  EXPECT_THROW(ifStmt->addChildren({{elseBranch, newElseBranch}}),
               std::invalid_argument);
}

TEST_F(IfStmtFixture, IfStmtAddChildSuccess) {  /* NOLINT */
  auto ifStmt = new If(condition, thenBranch);
  auto newElseBranch = new Block(new VarAssignm("a", new LiteralInt(1024)));
  ifStmt->removeChild(ifStmt->getElseBranch(), true);
  ifStmt->addChild(newElseBranch, true);

  // children
  EXPECT_EQ(ifStmt->getChildren().size(), 3);
  EXPECT_EQ(ifStmt->getCondition(), condition);
  EXPECT_EQ(ifStmt->getThenBranch(), thenBranch);
  EXPECT_EQ(ifStmt->getElseBranch(), newElseBranch);

  // parents
  EXPECT_EQ(condition->getParent()!=nullptr, 1);
  EXPECT_EQ(condition->getParent(), ifStmt);
  EXPECT_EQ(thenBranch->getParent()!=nullptr, 1);
  EXPECT_EQ(thenBranch->getParent(), ifStmt);
  EXPECT_EQ(newElseBranch->getParent()!=nullptr, 1);
  EXPECT_EQ(newElseBranch->getParent(), ifStmt);
}

TEST(ChildParentTests, LiteralBoolHasNoChildrenOrParents) {  /* NOLINT */
  // Literals should never have any children
  LiteralBool literalBool(true);
  ASSERT_TRUE(literalBool.getChildren().empty());
  ASSERT_EQ(literalBool.getParent(), nullptr);

  literalBool.setValue(false);
  ASSERT_TRUE(literalBool.getChildren().empty());
  ASSERT_EQ(literalBool.getParent(), nullptr);
}

TEST(ChildParentTests, LiteralFloatHasNoChildrenOrParents) {  /* NOLINT */
  // Literals should never have any children
  LiteralFloat literalFloat(true);
  ASSERT_TRUE(literalFloat.getChildren().empty());
  ASSERT_EQ(literalFloat.getParent(), nullptr);

  literalFloat.setValue(false);
  ASSERT_TRUE(literalFloat.getChildren().empty());
  ASSERT_EQ(literalFloat.getParent(), nullptr);
}

TEST(ChildParentTests, LiteralIntHasNoChildrenOrParents) {  /* NOLINT */
  // Literals should never have any children
  LiteralInt literalInt(33);
  ASSERT_TRUE(literalInt.getChildren().empty());
  ASSERT_EQ(literalInt.getParent(), nullptr);

  literalInt.setValue(111);
  ASSERT_TRUE(literalInt.getChildren().empty());
  ASSERT_EQ(literalInt.getParent(), nullptr);
}

TEST(ChildParentTests, LiteralStringHasNoChildrenOrParents) {  /* NOLINT */
  // Literals should never have any children
  LiteralString literalString("alpha");
  ASSERT_TRUE(literalString.getChildren().empty());
  ASSERT_EQ(literalString.getParent(), nullptr);

  literalString.setValue("beta");
  ASSERT_TRUE(literalString.getChildren().empty());
  ASSERT_EQ(literalString.getParent(), nullptr);
}

class LogicalExprFixture : public ::testing::Test {
 protected:
  LiteralInt *literalInt;
  LiteralInt *literalIntAnother;
  LiteralBool *literalBool;
  LogCompOp opSymb;
  Operator *operatorGreaterEqual;

  LogicalExprFixture() {
    literalInt = new LiteralInt(24);
    literalIntAnother = new LiteralInt(6245);
    literalBool = new LiteralBool(true);
    opSymb = LogCompOp::GREATER_EQUAL;
    operatorGreaterEqual = new Operator(opSymb);
  }
};

TEST_F(LogicalExprFixture, LogicalExprStandardConstructor) {  /* NOLINT */
  auto logicalExpr = new LogicalExpr(literalInt, opSymb, literalIntAnother);

  // children
  ASSERT_EQ(logicalExpr->getChildren().size(), 3);
  ASSERT_EQ(logicalExpr->getLeft(), literalInt);
  ASSERT_TRUE(reinterpret_cast<Operator *>(logicalExpr->getOperator())->equals(opSymb));
  ASSERT_EQ(logicalExpr->getRight(), literalIntAnother);

  // parents
  ASSERT_EQ(logicalExpr->getParent(), nullptr);
  ASSERT_EQ(logicalExpr->getLeft()->getParent()!=nullptr, 1);
  ASSERT_TRUE(logicalExpr->getLeft()->hasParent(logicalExpr));
  ASSERT_EQ(logicalExpr->getOperator()->getParent()!=nullptr, 1);
  ASSERT_TRUE(logicalExpr->getOperator()->hasParent(logicalExpr));
  ASSERT_EQ(logicalExpr->getRight()->getParent()!=nullptr, 1);
  ASSERT_TRUE(logicalExpr->getRight()->hasParent(logicalExpr));
}

TEST_F(LogicalExprFixture, LogicalExprEmptyConstructor) {  /* NOLINT */
  ArithmeticExpr logicalExpr;
  ASSERT_EQ(logicalExpr.getChildren().size(), 3);
  ASSERT_EQ(logicalExpr.countChildrenNonNull(), 0);
  ASSERT_EQ(logicalExpr.getParent(), nullptr);
}

TEST_F(LogicalExprFixture, LogicalExprOperatorOnlyConstructor) {  /* NOLINT */
  auto logicalExpr = new LogicalExpr(opSymb);

  // children
  ASSERT_EQ(logicalExpr->getChildren().size(), 3);
  ASSERT_EQ(logicalExpr->countChildrenNonNull(), 1);
  ASSERT_EQ(logicalExpr->getLeft(), nullptr);
  ASSERT_TRUE(reinterpret_cast<Operator *>(logicalExpr->getOperator())->equals(opSymb));
  ASSERT_EQ(logicalExpr->getRight(), nullptr);

  // parents
  ASSERT_EQ(logicalExpr->getParent(), nullptr);
  ASSERT_EQ(logicalExpr->getOperator()->getParent()!=nullptr, 1);
  ASSERT_TRUE(logicalExpr->getOperator()->hasParent(logicalExpr));
}

TEST_F(LogicalExprFixture, LogicalExprAddChildException_NoEmptyChildSpotAvailable) {  /* NOLINT */
  auto logicalExpr = new LogicalExpr(literalInt, opSymb, literalIntAnother);
  EXPECT_THROW(logicalExpr->addChild(new LiteralInt(3), false),
               std::logic_error);
}

TEST_F(LogicalExprFixture, LogicalExprAddChildException_TooManyChildrenAdded) {  /* NOLINT */
  auto logicalExpr = new LogicalExpr(literalInt, opSymb, literalIntAnother);
  EXPECT_THROW(logicalExpr->addChildren({{literalInt, literalIntAnother, new Operator(opSymb), literalBool}}),
               std::invalid_argument);
}

TEST_F(LogicalExprFixture, LogicalExprAddChildSuccess) {  /* NOLINT */
  auto logicalExpr = new LogicalExpr();
  logicalExpr->setAttributes(nullptr, operatorGreaterEqual, literalBool);
  logicalExpr->addChild(literalIntAnother, true);

  // children
  EXPECT_EQ(logicalExpr->getChildren().size(), 3);
  EXPECT_EQ(logicalExpr->getLeft(), literalIntAnother);
  EXPECT_EQ(logicalExpr->getOperator(), operatorGreaterEqual);
  EXPECT_TRUE(reinterpret_cast<Operator *>(logicalExpr->getLeft())->equals(opSymb));
  EXPECT_EQ(logicalExpr->getRight(), literalBool);

  // parents
  EXPECT_EQ(literalIntAnother->getParent()!=nullptr, 1);
  EXPECT_EQ(literalIntAnother->getParent(), logicalExpr);
  EXPECT_EQ(operatorGreaterEqual->getParent()!=nullptr, 1);
  EXPECT_EQ(operatorGreaterEqual->getParent(), logicalExpr);
  EXPECT_EQ(literalBool->getParent()!=nullptr, 1);
  EXPECT_EQ(literalBool->getParent(), logicalExpr);
}

TEST(ChildParentTests, OperatorHasNoChildrenOrParents) {  /* NOLINT */
  Operator op(LogCompOp::GREATER_EQUAL);
  ASSERT_TRUE(op.getChildren().empty());
  ASSERT_EQ(op.getParent(), nullptr);
}

class ReturnStatementFixture : public ::testing::Test {
 protected:
  AbstractExpr *abstractExpr;
  AbstractExpr *abstractExprOther;

  ReturnStatementFixture() {
    abstractExpr = new LiteralInt(22);
    abstractExprOther = new LiteralBool(true);
  }
};

TEST_F(ReturnStatementFixture, ReturnStatementStandardConstructor) {  /* NOLINT */
  auto returnStatement = new Return(abstractExpr);

  // children
  ASSERT_EQ(returnStatement->getChildren().size(), 1);
  ASSERT_EQ(returnStatement->getChildren().front(), abstractExpr);

  // parent
  ASSERT_EQ(returnStatement->getParent(), nullptr);
  ASSERT_EQ(returnStatement->getReturnExpressions().at(0)->getParent()!=nullptr, 1);
  ASSERT_TRUE(returnStatement->getReturnExpressions().at(0)->hasParent(returnStatement));
}

TEST_F(ReturnStatementFixture, ReturnStatementEmptyConstructor) {  /* NOLINT */
  Return returnStatement;
  ASSERT_EQ(returnStatement.getChildren().size(), 0);
  ASSERT_EQ(returnStatement.countChildrenNonNull(), 0);
  ASSERT_EQ(returnStatement.getParent(), nullptr);
}

TEST_F(ReturnStatementFixture, ReturnStatementAddSecondChild) {  /* NOLINT */
  auto returnStatement = new Return(abstractExpr);
  returnStatement->addChild(abstractExprOther, false);
  EXPECT_EQ(returnStatement->getChildren().size(), 2);
  EXPECT_EQ(returnStatement->getChildrenNonNull().size(), 2);
}

TEST_F(ReturnStatementFixture, ReturnStatementAddChildSuccess) {  /* NOLINT */
  auto returnStatement = new Return();
  returnStatement->addChild(abstractExprOther, true);
  EXPECT_EQ(returnStatement->getReturnExpressions().front(), abstractExprOther);
  EXPECT_EQ(returnStatement->getChildren().size(), 1);
  EXPECT_EQ(returnStatement->getChildren().front(), abstractExprOther);
  EXPECT_EQ(abstractExprOther->getParent()!=nullptr, 1);
  EXPECT_EQ(abstractExprOther->getParent(), returnStatement);
}

class UnaryExprFixture : public ::testing::Test {
 protected:
  UnaryOp opSymbNegation;
  LiteralBool *literalBoolTrue;

  UnaryExprFixture() {
    opSymbNegation = UnaryOp::NEGATION;
    new Operator(opSymbNegation);
    literalBoolTrue = new LiteralBool(true);
  }
};

TEST_F(UnaryExprFixture, UnaryExprStandardConstructor) {  /* NOLINT */
  auto unaryExpr = new UnaryExpr(opSymbNegation, literalBoolTrue);

  // children
  ASSERT_EQ(unaryExpr->getChildren().size(), 2);
  ASSERT_TRUE(reinterpret_cast<Operator *>(unaryExpr->getOperator())->equals(opSymbNegation));
  ASSERT_EQ(unaryExpr->getRight(), literalBoolTrue);

  // parents
  ASSERT_EQ(unaryExpr->getParent(), nullptr);
  ASSERT_TRUE(unaryExpr->getOperator()->hasParent(unaryExpr));
  ASSERT_TRUE(unaryExpr->getRight()->hasParent(unaryExpr));
}

TEST_F(UnaryExprFixture, UnaryExprAddChildException_NoEmptyChildSpotAvailable) {  /* NOLINT */
  auto unaryExpr = new UnaryExpr(opSymbNegation, literalBoolTrue);
  EXPECT_THROW(unaryExpr->addChild(new Operator(UnaryOp::NEGATION), false), std::logic_error);
}

TEST_F(UnaryExprFixture, UnaryExprAddChildException_TooManyChildrenAdded) {  /* NOLINT */
  auto unaryExpr = new UnaryExpr(opSymbNegation, literalBoolTrue);
  EXPECT_THROW(unaryExpr->addChildren({new Operator(UnaryOp::NEGATION), new LiteralBool(false)}),
               std::logic_error);
}

TEST_F(UnaryExprFixture, UnaryExprtion_AddChildSuccess) {  /* NOLINT */
  auto unaryExpr = new UnaryExpr(opSymbNegation, literalBoolTrue);

  unaryExpr->removeChild(unaryExpr->getOperator(), false);
  auto newOperator = new Operator(UnaryOp::NEGATION);
  unaryExpr->addChild(newOperator, true);

  // children
  EXPECT_EQ(unaryExpr->getChildren().size(), 2);
  EXPECT_EQ(*unaryExpr->getOperator(), *newOperator);
  EXPECT_TRUE(unaryExpr->getOperator()->equals(newOperator->getOperatorSymbol()));

  // parents
  EXPECT_EQ(unaryExpr->getParent(), nullptr);
  EXPECT_EQ(unaryExpr->getOperator()->getParent()!=nullptr, 1);
  EXPECT_TRUE(unaryExpr->getOperator()->hasParent(unaryExpr));
}

class VarAssignmFixture : public ::testing::Test {
 protected:
  LiteralInt *literalInt222;
  std::string variableIdentifier;

  VarAssignmFixture() {
    literalInt222 = new LiteralInt(222);
    variableIdentifier = "secretX";
  }
};

TEST_F(VarAssignmFixture, VarAssignmStandardConstructor) {  /* NOLINT */
  auto varAssignm = new VarAssignm(variableIdentifier, literalInt222);

  // children
  ASSERT_EQ(varAssignm->getChildren().size(), 1);
  ASSERT_EQ(varAssignm->getValue(), literalInt222);

  // parents
  ASSERT_EQ(varAssignm->getParent(), nullptr);
  ASSERT_EQ(varAssignm->getValue()->getParent()!=nullptr, 1);
  ASSERT_TRUE(varAssignm->getValue()->hasParent(varAssignm));
}

TEST_F(VarAssignmFixture, VarAssignm_NoEmptyChildSpotAvailable) {  /* NOLINT */
  auto varAssignm = new VarAssignm(variableIdentifier, literalInt222);
  EXPECT_THROW(varAssignm->addChild(new LiteralBool(true), false), std::logic_error);
}

TEST_F(VarAssignmFixture, VarAssignm_TooManyChildrenAdded) {  /* NOLINT */
  auto varAssignm = new VarAssignm(variableIdentifier, literalInt222);
  EXPECT_THROW(varAssignm->addChildren({new LiteralBool(true), new LiteralInt(5343)}), std::invalid_argument);
}

TEST_F(VarAssignmFixture, VarAssignmAddChildSuccess) {  /* NOLINT */
  auto varAssignm = new VarAssignm(variableIdentifier, literalInt222);

  varAssignm->removeChildren();
  auto newChild = new LiteralBool(false);
  varAssignm->addChild(newChild, true);

  // children
  ASSERT_EQ(varAssignm->getChildren().size(), 1);
  ASSERT_EQ(varAssignm->getValue(), newChild);

  // parents
  ASSERT_EQ(varAssignm->getParent(), nullptr);
  ASSERT_EQ(varAssignm->getValue()->getParent()!=nullptr, 1);
  ASSERT_TRUE(varAssignm->getValue()->hasParent(varAssignm));
}

class VarDeclFixture : public ::testing::Test {
 protected:
  int integerValue;
  float floatValue;
  bool boolValue;
  std::string stringValue;
  LiteralInt *literalInt;
  std::string variableIdentifier;
  Types datatypeInt;

  VarDeclFixture() {
    literalInt = new LiteralInt(integerValue);
    integerValue = 343224;
    variableIdentifier = "maxValue";
    floatValue = 2.42f;
    boolValue = false;
    stringValue = "Determines the maximum allowed value";
    datatypeInt = Types::INT;
  }

  static void checkExpected(VarDecl *varDeclaration, Datatype *expectedDatatype, AbstractExpr *expectedValue) {
    // children
    ASSERT_EQ(varDeclaration->getChildren().size(), 2);
    ASSERT_EQ(reinterpret_cast<Datatype *>(varDeclaration->getDatatype()), expectedDatatype);
    ASSERT_EQ(varDeclaration->getInitializer(), expectedValue);

    // parents
    ASSERT_EQ(varDeclaration->getParent(), nullptr);
    ASSERT_EQ(varDeclaration->getDatatype()->getParent()!=nullptr, 1);
    ASSERT_TRUE(varDeclaration->getDatatype()->hasParent(varDeclaration));
    ASSERT_EQ(varDeclaration->getInitializer()->getParent()!=nullptr, 1);
    ASSERT_TRUE(varDeclaration->getInitializer()->hasParent(varDeclaration));
  }
};

TEST_F(VarDeclFixture, VarDeclStandardConstructor) {  /* NOLINT */
  auto variableDeclaration = new VarDecl(variableIdentifier, datatypeInt, literalInt);
  ASSERT_EQ(reinterpret_cast<Datatype *>(variableDeclaration->getDatatype())->toString(false),
            Datatype(datatypeInt).toString(false));
  checkExpected(variableDeclaration, variableDeclaration->getDatatype(), literalInt);
}

TEST_F(VarDeclFixture, VarDeclIntConstructor) {  /* NOLINT */
  auto variableDeclaration = new VarDecl(variableIdentifier, integerValue);
  ASSERT_EQ(reinterpret_cast<LiteralInt *>(variableDeclaration->getInitializer())->getValue(), integerValue);
  ASSERT_EQ(reinterpret_cast<Datatype *>(variableDeclaration->getDatatype())->toString(false),
            Datatype(Types::INT).toString(false));
  checkExpected(variableDeclaration, variableDeclaration->getDatatype(), variableDeclaration->getInitializer());
}

TEST_F(VarDeclFixture, VarDeclBoolConstructor) {  /* NOLINT */
  auto variableDeclaration = new VarDecl(variableIdentifier, boolValue);
  ASSERT_EQ(reinterpret_cast<LiteralBool *>(variableDeclaration->getInitializer())->getValue(), boolValue);
  ASSERT_EQ(reinterpret_cast<Datatype *>(variableDeclaration->getDatatype())->toString(false),
            Datatype(Types::BOOL).toString(false));
  checkExpected(variableDeclaration, variableDeclaration->getDatatype(), variableDeclaration->getInitializer());
}

TEST_F(VarDeclFixture, VarDeclFloatConstructor) {  /* NOLINT */
  auto variableDeclaration = new VarDecl(variableIdentifier, floatValue);
  ASSERT_EQ(reinterpret_cast<LiteralFloat *>(variableDeclaration->getInitializer())->getValue(), floatValue);
  ASSERT_EQ(reinterpret_cast<Datatype *>(variableDeclaration->getDatatype())->toString(false),
            Datatype(Types::FLOAT).toString(false));
  checkExpected(variableDeclaration, variableDeclaration->getDatatype(), variableDeclaration->getInitializer());
}

TEST_F(VarDeclFixture, VarDeclStringConstructor) {  /* NOLINT */
  auto variableDeclaration = new VarDecl(variableIdentifier, stringValue);
  ASSERT_EQ(reinterpret_cast<LiteralString *>(variableDeclaration->getInitializer())->getValue(), stringValue);
  ASSERT_EQ(reinterpret_cast<Datatype *>(variableDeclaration->getDatatype())->toString(false),
            Datatype(Types::STRING).toString(false));
  checkExpected(variableDeclaration, variableDeclaration->getDatatype(), variableDeclaration->getInitializer());
}

TEST(ChildParentTests, Variable) {  /* NOLINT */
  Variable variable("myInt");
  ASSERT_TRUE(variable.getChildren().empty());
  ASSERT_EQ(variable.getParent(), nullptr);
}

class WhileStmtFixture : public ::testing::Test {
 protected:
  AbstractExpr *whileCondition;
  AbstractStatement *whileBlock;

 public:
  WhileStmtFixture() {
    whileCondition = new LogicalExpr(new LiteralInt(32), GREATER_EQUAL, new Variable("a"));
    whileBlock = new Block();
  }
};

TEST_F(WhileStmtFixture, WhileStandardConstructor) {  /* NOLINT */
  auto whileStmt = new While(whileCondition, whileBlock);

  EXPECT_EQ(whileStmt->getMaxNumberChildren(), 2);

  // children
  EXPECT_EQ(whileStmt->countChildrenNonNull(), 2);
  EXPECT_EQ(whileStmt->getCondition(), whileCondition);
  EXPECT_EQ(whileStmt->getBody(), whileBlock);

  // parents
  EXPECT_TRUE(whileCondition->hasParent());
  EXPECT_TRUE(whileCondition->hasParent(whileStmt));
  EXPECT_TRUE(whileBlock->hasParent());
  EXPECT_TRUE(whileBlock->hasParent(whileStmt));
}

class ForLoopFixture : public ::testing::Test {
 protected:
  AbstractStatement *forInitializer;
  AbstractStatement *forUpdate;
  AbstractStatement *forBody;
  LogicalExpr *forCondition;

 public:
  ForLoopFixture() {
    // int = 0;
    forInitializer = new Block(new VarDecl("i", Types::INT, new LiteralInt(0)));
    // i < 3
    forCondition = new LogicalExpr(new Variable("i"), SMALLER, new LiteralInt(3));
    // i = i+1
    forUpdate = new Block(new VarAssignm("i", new ArithmeticExpr(new Variable("i"), ADDITION, new LiteralInt(1))));
    // sum = sum + base * i;
    forBody = new Block(
        new VarAssignm("sum",
                       new ArithmeticExpr(
                           new Variable("sum"),
                           ADDITION,
                           new ArithmeticExpr(
                               new Variable("base"),
                               MULTIPLICATION,
                               new Variable("i")))));
  }
};

TEST_F(ForLoopFixture, ForStmtStandardConstructor) {  /* NOLINT */
  auto forStmt = new For(forInitializer, forCondition, forUpdate, forBody);

  // children
  EXPECT_EQ(forStmt->getChildren().size(), 4);
  EXPECT_EQ(forStmt->getInitializer(), forInitializer);
  EXPECT_EQ(forStmt->getCondition(), forCondition);
  EXPECT_EQ(forStmt->getUpdate(), forUpdate);
  EXPECT_EQ(forStmt->getBody(), forBody);

  // parents
  EXPECT_EQ(forStmt->getParent(), nullptr);
  EXPECT_TRUE(forStmt->getCondition()->hasParent());
  EXPECT_TRUE(forStmt->getCondition()->hasParent(forStmt));
  EXPECT_TRUE(forStmt->getInitializer()->hasParent());
  EXPECT_TRUE(forStmt->getInitializer()->hasParent(forStmt));
  EXPECT_TRUE(forStmt->getBody()->hasParent());
  EXPECT_TRUE(forStmt->getBody()->hasParent(forStmt));
  EXPECT_TRUE(forStmt->getUpdate()->hasParent());
  EXPECT_TRUE(forStmt->getUpdate()->hasParent(forStmt));
}

TEST_F(ForLoopFixture, ForStmt_NoEmptyChildSpotAvailable) {  /* NOLINT */
  auto forStmt = new For(forInitializer, forCondition, forUpdate, forBody);
  EXPECT_THROW(forStmt->addChild(new VarAssignm("a", new LiteralInt(1))), std::logic_error);
}

TEST_F(ForLoopFixture, ForStmtAddChildSuccess) {  /* NOLINT */
  auto forStmt = new For(forInitializer, forCondition, forUpdate, forBody);
  forStmt->removeChild(forBody);

  EXPECT_TRUE(forBody->hasParent());
  EXPECT_EQ(forStmt->getChildrenNonNull().size(), 3);

  auto newChild = new VarAssignm("x", new ArithmeticExpr(new Variable("x"), MULTIPLICATION, new LiteralInt(2)));
  forStmt->addChild(newChild);
  EXPECT_EQ(forStmt->getBody()->getStatements().at(0), newChild);
  EXPECT_TRUE(newChild->hasParent());
  EXPECT_TRUE(newChild->hasParent(forStmt));
}

TEST(OperatorExpr, OperatorExprStandardConstructor) {  /* NOLINT */
  auto opAddition = new Operator(ADDITION);
  auto literalTwo = new LiteralInt(2);
  auto literal444 = new LiteralInt(444);
  auto literalOne = new LiteralInt(1);
  auto opExpr = new OperatorExpr(opAddition, {literalTwo, literal444, literalOne});

  // children
  EXPECT_EQ(opExpr->getChildren().size(), 2);
  EXPECT_EQ(opExpr->getOperator(), opAddition);
  EXPECT_EQ(*opExpr->getOperands().at(0)->castTo<LiteralInt>(), *new LiteralInt(447));

  // parents
  EXPECT_EQ(opExpr->getParent(), nullptr);
  EXPECT_TRUE(opExpr->getOperator()->hasParent());
  EXPECT_TRUE(opExpr->getOperator()->hasParent(opExpr));
  EXPECT_TRUE(opExpr->getLeft()->hasParent());
  EXPECT_TRUE(opExpr->getLeft()->hasParent(opExpr));
}

TEST(OperatorExpr, OperatorExprSingleArgConstructor) {  /* NOLINT */
  auto opSubtraction = new Operator(SUBTRACTION);
  auto opExpr = new OperatorExpr(opSubtraction);

  // children
  EXPECT_EQ(opExpr->getChildren().size(), 1);
  EXPECT_EQ(opExpr->getOperator(), opSubtraction);

  // parents
  EXPECT_EQ(opExpr->getParent(), nullptr);
  EXPECT_TRUE(opExpr->getOperator()->hasParent());
  EXPECT_TRUE(opExpr->getOperator()->hasParent(opExpr));
}

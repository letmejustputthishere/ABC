#include "AstTestingGenerator.h"
#include "ast_opt/ast/Ast.h"
#include "ast_opt/ast/Function.h"
#include "ast_opt/ast/ArithmeticExpr.h"
#include "ast_opt/ast/VarAssignm.h"
#include "gtest/gtest.h"

class AstTestFixture : public ::testing::Test {
 protected:
  Ast ast;

  void SetUp() override {
    // This AST corresponds to the program:
    // int computePrivate(int inputA, int inputB, int inputC) {
    //   int prod = inputA * [inputB * inputC]
    //   return prod / 3;
    // }
    AstTestingGenerator::generateAst(7, ast);
  }
};

TEST_F(AstTestFixture, deleteNode_deleteSingleLeafNodeOnly) { /* NOLINT */
  // retrieve the arithmetic expression of interest
  auto func = dynamic_cast<Function *>(ast.getRootNode());
  ASSERT_NE(func, nullptr);
  auto arithmeticExpr =
      dynamic_cast<ArithmeticExpr *>(func->getBodyStatements().at(0)->castTo<VarAssignm>()->getValue());
  ASSERT_NE(arithmeticExpr, nullptr);

  // retrieve the deletion target -> variable of the arithmetic expression
  auto variable = static_cast<AbstractNode*>(arithmeticExpr->getLeft());
  ASSERT_NE(variable, nullptr);
  ASSERT_EQ(variable->getParent(), arithmeticExpr);

  // delete node and verify deletion success
  ast.deleteNode(&variable);
  ASSERT_EQ(arithmeticExpr->getLeft(), nullptr);
  ASSERT_EQ(variable, nullptr);
}

TEST_F(AstTestFixture, deleteNode_deleteRecursiveSubtreeNonEmpty) { /* NOLINT */
  // retrieve the arithmetic expression of interest
  auto func = dynamic_cast<Function *>(ast.getRootNode());
  ASSERT_NE(func, nullptr);
  auto arithmeticExprParent = func->getBodyStatements().at(0)->castTo<VarAssignm>()->getValue()->castTo<ArithmeticExpr>();
  auto arithmeticExpr = dynamic_cast<ArithmeticExpr *>(arithmeticExprParent->getRight());
  ASSERT_NE(arithmeticExpr, nullptr);

  // save and check node's children
  const auto &arithmeticExprChildren = arithmeticExpr->getChildren();
  for (auto &c : arithmeticExprChildren)
    ASSERT_EQ(c->getParent(), arithmeticExpr);

  // delete node and its subtree and verify deletion success
  AbstractNode *arithmeticExprPtr = arithmeticExpr;
  ast.deleteNode(&arithmeticExprPtr, true);
  // verify that ArithmeticExpr was deleted, also from its parent
  ASSERT_EQ(arithmeticExprPtr, nullptr);
  ASSERT_EQ(arithmeticExprParent->getRight(), nullptr);
  //TODO: verify that children are deleted
}

TEST_F(AstTestFixture, deleteNode_deleteRecursiveSubtreeEmpty) { /* NOLINT */
  // The same test as deleteNode_deleteSingleLeafNodeOnly but now we use the 'deleteSubtreeRecursively' flag
  // this should not change anything though if there are no children present

  // retrieve the arithmetic expression of interest
  auto func = dynamic_cast<Function *>(ast.getRootNode());
  ASSERT_NE(func, nullptr);
  auto arithmeticExpr =
      dynamic_cast<ArithmeticExpr *>(func->getBodyStatements().at(0)->castTo<VarAssignm>()->getValue()->castTo<ArithmeticExpr>()->getRight());
  ASSERT_NE(arithmeticExpr, nullptr);

  // retrieve the deletion target -> variable of the arithmetic expression
  auto variable = static_cast<AbstractNode*>(arithmeticExpr->getLeft());
  ASSERT_NE(variable, nullptr);
  ASSERT_EQ(variable->getParent(), arithmeticExpr);

  // delete node and verify deletion success
  ast.deleteNode(&variable, true);
  ASSERT_EQ(arithmeticExpr->getLeft(), nullptr);
  ASSERT_EQ(variable, nullptr);
}

TEST_F(AstTestFixture, deleteNode_ChildrenExisting) { /* NOLINT */
  // The same test as deleteNode_deleteRecursiveSubtreeNonEmpty but now we simulate forgetting the use of the
  // 'deleteSubtreeRecursively' flag which should throw an exception

  // retrieve the arithmetic expression of interest
  auto func = dynamic_cast<Function *>(ast.getRootNode());
  ASSERT_NE(func, nullptr);
  auto arithmeticExprParent = func->getBodyStatements().at(0)->castTo<VarAssignm>()->getValue()->castTo<ArithmeticExpr>();
  auto arithmeticExpr = dynamic_cast<ArithmeticExpr *>(arithmeticExprParent->getRight());
  ASSERT_NE(arithmeticExpr, nullptr);

  // save and check node's children
  int arithmeticExprNumChildren = arithmeticExpr->countChildrenNonNull();
  for (auto &c : arithmeticExpr->getChildrenNonNull())
    ASSERT_EQ(c->getParent(), arithmeticExpr);

  // delete node and its subtree and verify deletion success
  AbstractNode *arithmeticExprPtr = arithmeticExpr;
  // by using the default parameter value for deleteSubtreeRecursively
  EXPECT_THROW(ast.deleteNode(&arithmeticExprPtr), std::logic_error);
  // by expliciting passing the parameter value for deleteSubtreeRecursively
  EXPECT_THROW(ast.deleteNode(&arithmeticExprPtr, false), std::logic_error);

  // verify that ArithmeticExpr was not deleted, also from its parent
  ASSERT_NE(arithmeticExprPtr, nullptr);
  ASSERT_TRUE(arithmeticExprParent->hasChild(arithmeticExpr));
  // verify that children are deleted
  ASSERT_EQ(arithmeticExpr->countChildrenNonNull(), arithmeticExprNumChildren);
}

TEST_F(AstTestFixture, deepCopy) { /* NOLINT */
  // Test that copying an AST object properly performs a deep copy
  auto number_of_nodes = ast.getAllNodes().size();
  if (number_of_nodes!=0) {
    Ast copy = ast;

    // Delete all nodes in the copy
    auto rootNode = copy.getRootNode();
    copy.deleteNode(&rootNode,true);

    // Ensure that the copy is empty
    EXPECT_EQ(copy.getRootNode(), nullptr);
    ASSERT_TRUE(copy.getAllNodes().empty());

    // Ensure that original still has all nodes
    EXPECT_EQ(ast.getAllNodes().size(), number_of_nodes);

  } else {
    GTEST_SKIP_("Cannot perform deep copy test on empty AST");
  }
}

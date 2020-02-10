#include <gtest/gtest.h>
#include <include/utilities/TestUtils.h>
#include "AstTestingGenerator.h"
#include "CKKSRewriter.h"

TEST(CKKSOptimizerTest, EquivalenceTest) {
    for(int id = 1; id < AstTestingGenerator::getLargestId(); ++id) {
        /// Current AST
        Ast original_ast;
        AstTestingGenerator::generateAst(id, original_ast);

        /// Only test if AST is circuit-y
        if (original_ast.isValidCircuit()) {

            /// New AST for rewriting
            Ast ast;
            AstTestingGenerator::generateAst(id, ast);
            auto rewriter = CKKSRewriter(ast);
            rewriter.applyCKKSRewriting();

            /// Ensure ASTs are equivalent
            //TODO(pjattke): Implement AST-equivalence-helper

        } else {
            // If not a circuit, the rewriting should fail
            auto rewriter = CKKSRewriter( original_ast);
            EXPECT_THROW(rewriter.applyCKKSRewriting(),std::invalid_argument);
        }

    }
}
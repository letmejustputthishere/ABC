#include <gtest/gtest.h>
#include <include/utilities/TestUtils.h>
#include "AstTestingGenerator.h"
#include "CKKSRewriter.h"

TEST(CKKSOptimizerTest, EquivalenceTest) {
    for(int id = 0; id < AstTestingGenerator::getLargestId(); ++id) {
        /// Current AST
        Ast original_ast;
        AstTestingGenerator::generateAst(id, original_ast);

        /// Only test if AST is circuit-y
        if (original_ast.isValidCircuit()) {

            /// Copy of AST for rewriting
            Ast ast = original_ast;
            auto rewriter = CKKSRewriter(ast);
            rewriter.applyCKKSRewriting();

            /// Ensure ASTs are equivalent
            //TODO(pjattke): Implement AST-equivalence-helper
        }

    }
}
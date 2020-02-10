#include "CKKSRewriter.h"
#include <exception>

CKKSRewriter::CKKSRewriter(Ast &ast) : ast(ast), multiplicativeDepthCalculator(ast) {
    // Saves the AST, and initializes the multiplicative depth calculator
    // The latter currently already computes the depths, which is somewhat a wasted effort
}

Ast &CKKSRewriter::applyCKKSRewriting() {
    // Ensure that the Ast is already in circuit form
    if (!ast.isValidCircuit()) {
        throw std::invalid_argument("Ast must be a valid circuit for CKKS rewriting.");
    }

    // Update the multiplicative depth calculator, in case the tree was modified since this instance was created
    multiplicativeDepthCalculator.precomputeMultDepths(ast);



    //TODO(vianda) Implement CKKS Rewriting
    throw std::runtime_error("Not Implemented");
}

#include "CKKSRewriter.h"
#include <exception>
#include "LiteralFloat.h"
#include "LiteralInt.h"
#include "LiteralBool.h"


CKKSRewriter::CKKSRewriter(Ast &ast) : ast(ast), multiplicativeDepthCalculator(ast) {
    // Saves the AST, and initializes the multiplicative depth calculator
    // The latter currently already computes the depths, which is somewhat a wasted effort
}

Ast &CKKSRewriter::applyCKKSRewriting() {
    // Ensure that the Ast is already in circuit form
    if (!ast.isValidCircuit()) {
        throw std::invalid_argument("Ast must be a valid circuit for CKKS rewriting.");
    }

    // Flip the ast, to correspond better to the EVA paper
    if (!ast.isReversed()) { ast.reverseEdges(); };

    // Update the multiplicative depth calculator, in case the tree was modified since this instance was created
    multiplicativeDepthCalculator.precomputeMultDepths(ast);

    // Initialize scales of input nodes
    setInputScales();

    /// the "waterline", i.e. the maximum scale of any inputs into the circuit
    auto sf = waterline();


    //TODO(vianda) Implement CKKS Rewriting
    throw std::runtime_error("Not Implemented");
}


void CKKSRewriter::setInputScales() {
    //TODO(vianda): How to determine the initial scales? This task is left to the developer in EVA
    // For now, we will simply assume 2^40 for everything, but we should be calculating these based on ...something?

    auto inputScaleSetter = InputScaleSetter(scales);
    inputScaleSetter.visit(ast);


}

uint CKKSRewriter::waterline() {

}


// InputScaleSetter
InputScaleSetter::InputScaleSetter(std::unordered_map<std::string, int> &scales) : scales(scales) {}

void InputScaleSetter::visit(LiteralFloat &elem) {
    scales.insert({elem.getUniqueNodeId(), 40});
}

void InputScaleSetter::visit(LiteralInt &elem) {
    scales.insert({elem.getUniqueNodeId(), 40});
}

void InputScaleSetter::visit(LiteralBool &elem) {
    scales.insert({elem.getUniqueNodeId(), 40});
}
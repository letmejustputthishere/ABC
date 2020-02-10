#ifndef AST_OPTIMIZER_CKKSREWRITER_H
#define AST_OPTIMIZER_CKKSREWRITER_H

#include <string>
#include <unordered_map>
#include "Ast.h"
#include "MultiplicativeDepthCalculator.h"
#include "Visitor.h"

/// Introduces the ciphertext maintenance operations (rescale, relinearize, modulus switching, etc)
/// required in the CKKS Scheme (CKKS 2017) into an AST, i.e. decides when during the computation to apply them.
/// This is done in a near-optimal way, based on ideas from EVA (DRKS+ 2019)
///
/// Cheon, Jung Hee, Andrey Kim, Miran Kim, and Yongsoo Song. 2017.
/// “Homomorphic Encryption for Arithmetic of Approximate Numbers.”
/// In Advances in Cryptology – ASIACRYPT 2017, 409–37. Springer International Publishing.
///
/// Dathathri, Roshan, Blagovesta Kostova, Olli Saarikivi, Wei Dai, Kim Laine, and Madanlal Musuvathi. 2019.
/// “EVA: An Encrypted Vector Arithmetic Language and Compiler for Efficient Homomorphic Computation.”
/// arXiv [cs.CR]. arXiv. http://arxiv.org/abs/1912.11951.
///
class CKKSRewriter {
public:
    /// Initializes the CKKSRewriter with an Ast to modify
    /// The Ast must be a circuit for CKKSRewriting to be successful
    /// \param ast
    explicit CKKSRewriter(Ast &ast);

    /// Add ciphertext maintenance operations required for CKKS to the AST
    /// This modifies the AST that was passed when the object was created
    /// \return Reference to the rewritten AST
    /// \throws std::invalid_argument If the Ast in the CKKSRewriter instance is not a valid circuit
    Ast &applyCKKSRewriting();

    /// Chain of moduli, where the i-th element represents the number of bits of the i-th prime in the chain
    typedef std::vector<size_t> Modulus;

private:
    /// Current AST to be rewritten
    Ast &ast;

    /// Helper to compute and keep track of each node's multiplicative depth
    MultiplicativeDepthCalculator multiplicativeDepthCalculator;

    /// Dictionary to keep track of the modulus chain
    std::unordered_map<std::string, Modulus> moduli;

    /// Dictionary to keep track of the logarithm of the scale
    std::unordered_map<std::string, int> scales;

    /// Dictionary to keep track of the size (number of elements) of the ciphertexts
    std::unordered_map<std::string, uint> sizes;

    /// Sets the scale of input variables
    void setInputScales();

    /// Compute the "waterline" scale, i.e. the maximum scale of any input into the circuit
    uint waterline();
};

/// Helper Class to Set Input Scales
class InputScaleSetter : public Visitor {
private:
    std::unordered_map<std::string, int> &scales;

public:
    explicit InputScaleSetter(std::unordered_map<std::string, int> &scales);

    using Visitor::visit;

    void visit(LiteralInt &elem) override;

    void visit(LiteralFloat &elem) override;

    void visit(LiteralBool &elem) override;
};


#endif //AST_OPTIMIZER_CKKSREWRITER_H

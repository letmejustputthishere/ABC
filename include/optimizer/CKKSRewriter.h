#ifndef AST_OPTIMIZER_CKKSREWRITER_H
#define AST_OPTIMIZER_CKKSREWRITER_H

#include "Ast.h"
#include "MultiplicativeDepthCalculator.h"

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

private:
    Ast & ast;
    MultiplicativeDepthCalculator multiplicativeDepthCalculator;

};


#endif //AST_OPTIMIZER_CKKSREWRITER_H

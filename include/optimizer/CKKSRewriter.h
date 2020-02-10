#ifndef AST_OPTIMIZER_CKKSREWRITER_H
#define AST_OPTIMIZER_CKKSREWRITER_H

#include "Ast.h"

class CKKSRewriter {
public:
    explicit CKKSRewriter(Ast& ast);

    Ast& applyCKKSRewriting();

};


#endif //AST_OPTIMIZER_CKKSREWRITER_H

#ifndef AST_OPTIMIZER_VISITOR_H
#define AST_OPTIMIZER_VISITOR_H
#include "IVisitor.h"


class Visitor : public IVisitor {
public:
    void visit(BinaryExpr &elem) override;

    void visit(Block &elem) override;

    void visit(Call &elem) override;

    void visit(CallExternal &elem) override;

    void visit(Function &elem) override;

    void visit(FunctionParameter &elem) override;

    void visit(Group &elem) override;

    void visit(If &elem) override;

    void visit(LiteralBool &elem) override;

    void visit(LiteralInt &elem) override;

    void visit(LiteralString &elem) override;

    void visit(LiteralFloat &elem) override;

    void visit(LogicalExpr &elem) override;

    void visit(Operator &elem) override;

    void visit(Return &elem) override;

    void visit(UnaryExpr &elem) override;

    void visit(VarAssignm &elem) override;

    void visit(VarDecl &elem) override;

    void visit(Variable &elem) override;

    void visit(While &elem) override;

    void visit(Ast &elem) override;

    Scope* curScope;

    void changeToOuterScope();

    void changeToInnerScope(const std::string &nodeId);
};

#endif //AST_OPTIMIZER_VISITOR_H
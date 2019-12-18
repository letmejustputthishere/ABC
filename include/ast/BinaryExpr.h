#ifndef MASTER_THESIS_CODE_BINARYEXPR_H
#define MASTER_THESIS_CODE_BINARYEXPR_H

#include "Operator.h"
#include "AbstractExpr.h"
#include "Literal.h"
#include "LiteralInt.h"
#include "LiteralBool.h"
#include "LiteralString.h"

class BinaryExpr : public AbstractExpr, public Node {
protected:
    AbstractExpr *left;
    Operator *op;
    AbstractExpr *right;

public:
    /// Represents an expression of the form "left op right", e.g., "2 + a" or "53 * 3".
    /// \param left is the left operand of the expression.
    /// \param op is the operator of the expression.
    /// \param right is the right operand of the expression.
    BinaryExpr(AbstractExpr *left, OpSymb::BinaryOp op, AbstractExpr *right);

    template<typename T1, typename T2>
    BinaryExpr(T1 left, OpSymb::BinaryOp op, T2 right) {
        this->left = createParam(left);
        this->op = new Operator(op);
        this->right = createParam(right);
    }

    explicit BinaryExpr(Operator *op);

    [[nodiscard]] json toJson() const override;

    ~BinaryExpr() override;

    [[nodiscard]] AbstractExpr *getLeft() const;

    [[nodiscard]] Operator &getOp() const;

    [[nodiscard]] AbstractExpr *getRight() const;

    void accept(Visitor &v) override;

    [[nodiscard]] std::string getNodeName() const override;

    BinaryExpr *containsValuesFrom(BinaryExpr *bexpTemplate);

    void setLeft(AbstractExpr *left);

    void setOp(Operator *op);

    void setRight(AbstractExpr *right);

    void static swapOperandsLeftAWithRightB(BinaryExpr *bexpA, BinaryExpr *bexpB);
};


#endif //MASTER_THESIS_CODE_BINARYEXPR_H

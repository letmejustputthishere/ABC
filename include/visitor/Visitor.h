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


/// SFINAE based detection if T::visit(Args...) exists
template<typename T, typename... Args>
class is_visit_available {
    template<typename C,
            typename = decltype(std::declval<C>().visit(std::declval<Args>()...))>
    static std::true_type test(int);

    template<typename C>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

/// Does T::visit(Args...) exist? (Syntactic sugar for is_visit_available)
template<typename T, typename... Args>
constexpr bool has_visit = is_visit_available<T, Args...>::value;

/// Helper Class to create Visitors. SpecialVisitor should inherit IVisitor publicly,
/// should define at least one visit(Some AST Type) (hiding all base class functions) and NOT do using IVisitor::visit
template <typename SpecialVisitor>
class VisitorHelper : public SpecialVisitor {
public:
    /// Ensure that SpecialVisitor is actually a visitor
    static_assert(std::is_base_of<IVisitor,SpecialVisitor>::value);

    /// Inherit Constructors from SpecialVisitor
    using SpecialVisitor::SpecialVisitor;

    void visit(BinaryExpr &elem) override {
        if constexpr (has_visit<SpecialVisitor,BinaryExpr>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }

    }

    void visit(Block &elem) override {
        if constexpr (has_visit<SpecialVisitor,Block>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(Call &elem) override {
        if constexpr (has_visit<SpecialVisitor,Call>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(CallExternal &elem) override {
        if constexpr (has_visit<SpecialVisitor,CallExternal>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(Function &elem) override {
        if constexpr (has_visit<SpecialVisitor,BinaryExpr>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(FunctionParameter &elem) override {
        if constexpr (has_visit<SpecialVisitor,FunctionParameter>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(Group &elem) override {
        if constexpr (has_visit<SpecialVisitor,Group>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(If &elem) override {
        if constexpr (has_visit<SpecialVisitor,If>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(LiteralBool &elem) override {
        if constexpr (has_visit<SpecialVisitor,LiteralBool>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(LiteralInt &elem) override {
        if constexpr (has_visit<SpecialVisitor,LiteralInt>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(LiteralString &elem) override {
        if constexpr (has_visit<SpecialVisitor,LiteralString>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(LiteralFloat &elem) override {
        if constexpr (has_visit<SpecialVisitor,LiteralFloat>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(LogicalExpr &elem) override {
        if constexpr (has_visit<SpecialVisitor,LogicalExpr>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(Operator &elem) override {
        if constexpr (has_visit<SpecialVisitor,Operator>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(Return &elem) override {
        if constexpr (has_visit<SpecialVisitor,Return>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(UnaryExpr &elem) override {
        if constexpr (has_visit<SpecialVisitor,UnaryExpr>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(VarAssignm &elem) override {
        if constexpr (has_visit<SpecialVisitor,VarAssignm>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(VarDecl &elem) override {
        if constexpr (has_visit<SpecialVisitor,VarDecl>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(Variable &elem) override {
        if constexpr (has_visit<SpecialVisitor,Variable>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(While &elem) override {
        if constexpr (has_visit<SpecialVisitor,While>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }

    void visit(Ast &elem) override {
        if constexpr (has_visit<SpecialVisitor,Ast>)  {
            this->SpecialVisitor::visit(elem);
        } else {
            this->Visitor::visit(elem);
        }
    }
};

#endif //AST_OPTIMIZER_VISITOR_H
#ifndef AST_OPTIMIZER_IVISITOR_H
#define AST_OPTIMIZER_IVISITOR_H

#include <string>

class AbstractExpr;

class AbstractStatement;

class Ast;

class BinaryExpr;

class Block;

class Call;

class CallExternal;

class Class;

class Function;

class FunctionParameter;

class If;

class Literal;

class LiteralBool;

class LiteralInt;

class LiteralString;

class LiteralFloat;

class LogicalExpr;

class Operator;

class Return;

class UnaryExpr;

class VarAssignm;

class VarDecl;

class Variable;

class While;

class Scope;

class IVisitor {
 public:
  virtual void visit(BinaryExpr &elem) = 0;

  virtual void visit(Block &elem)= 0;

  virtual void visit(Call &elem)= 0;

  virtual void visit(CallExternal &elem)= 0;

  virtual void visit(Function &elem) = 0;

  virtual void visit(FunctionParameter &elem) = 0;

  virtual void visit(If &elem) = 0;

  virtual void visit(LiteralBool &elem) = 0;

  virtual void visit(LiteralInt &elem) = 0;

  virtual void visit(LiteralString &elem) = 0;

  virtual void visit(LiteralFloat &elem) = 0;

  virtual void visit(LogicalExpr &elem) = 0;

  virtual void visit(Operator &elem) = 0;

  virtual void visit(Return &elem) = 0;

  virtual void visit(UnaryExpr &elem) = 0;

  virtual void visit(VarAssignm &elem) = 0;

  virtual void visit(VarDecl &elem) = 0;

  virtual void visit(Variable &elem) = 0;

  virtual void visit(While &elem) = 0;

  /// This and only this method should be used to traverse an AST.
  /// \param elem A reference to the Abstract Syntax Tree (AST).
  virtual void visit(Ast &elem) = 0;

};

#endif //AST_OPTIMIZER_IVISITOR_H

#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARDECL_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARDECL_H_

#include <string>
#include "AbstractStatement.h"
#include "AbstractExpr.h"
#include "Datatype.h"

class VarDecl : public AbstractStatement {
 private:
  std::string identifier;

 public:
  VarDecl(std::string name, Datatype *datatype);

  VarDecl(std::string name, Types datatype, AbstractExpr *initializer);

  VarDecl(std::string name, Datatype *datatype, AbstractExpr *initializer);

  VarDecl(const std::string &name, void *abstractExpr);

  VarDecl(std::string name, int value);

  VarDecl(std::string name, bool value);

  VarDecl *clone() const override;

  /// This is just a helper constructor that allows to call VarDecl("randomString", "aiermkr");
  /// without this constructor the call will wrongly be forwarded to the VarDecl(std::string, bool) constructor.
  /// See https://stackoverflow.com/q/14770252/3017719.
  /// \param name The variable's identifier.
  /// \param valueAssignedTo The value assigned to the variable.
  VarDecl(std::string name, const char *valueAssignedTo);

  VarDecl(std::string name, float value);

  VarDecl(std::string name, std::string value);

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] const std::string &getIdentifier() const;

  [[nodiscard]] Datatype *getDatatype() const;

  [[nodiscard]] AbstractExpr *getInitializer() const;

  AbstractBinaryExpr *contains(AbstractBinaryExpr *aexpTemplate, ArithmeticExpr *excludedSubtree) override;

  ~VarDecl() override;

  std::string getVarTargetIdentifier() const;

  bool isEqual(AbstractStatement *as) override;

  void setAttributes(std::string varIdentifier, Datatype *datatype, AbstractExpr *value);

  int getMaxNumberChildren() override;

  [[nodiscard]] std::string toString(bool printChildren) const override;
  const std::vector<AbstractNode *> &getChildren() const override;
  void removeChildren() override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_VARDECL_H_

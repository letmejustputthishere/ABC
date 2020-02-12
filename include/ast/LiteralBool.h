#ifndef AST_OPTIMIZER_LITERALBOOL_H
#define AST_OPTIMIZER_LITERALBOOL_H

#include "Literal.h"
#include <string>
#include <map>

class LiteralBool : public Literal {
 private:
  bool value;

 public:
  explicit LiteralBool(bool value);

  ~LiteralBool() override;

  [[nodiscard]] json toJson() const override;

  void accept(IVisitor &v) override;

  [[nodiscard]] bool getValue() const;

  [[nodiscard]] std::string getTextValue() const;

  [[nodiscard]] std::string getNodeName() const override;

  Literal* evaluate(Ast &ast) override;

  bool operator==(const LiteralBool &rhs) const;

  bool operator!=(const LiteralBool &rhs) const;

  void addLiteralValue(std::string identifier, std::map<std::string, Literal*> &paramsMap) override;

  void setRandomValue(RandLiteralGen &rlg) override;
  void setValue(bool newValue);

  std::string toString() const override;

 protected:
  void print(std::ostream &str) const override;
  bool supportsCircuitMode() override;
};

#endif //AST_OPTIMIZER_LITERALBOOL_H

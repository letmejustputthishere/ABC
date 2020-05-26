#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_DATATYPE_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_DATATYPE_H_

#include <string>
#include <map>
#include "AbstractNode.h"

enum class Types {
  INT, FLOAT, STRING, BOOL
};

class Datatype : public AbstractNode {
 private:
  Types val;
  bool encrypted{false};

 public:
  [[nodiscard]] std::string getNodeType() const override;

  Datatype *clone() const override;

  explicit Datatype(Types di) : val(di) {}

  explicit Datatype(Types di, bool isEncrypted) : val(di), encrypted(isEncrypted) {}

  explicit Datatype(const std::string &type);

  static std::string enumToString(Types identifiers);

  explicit operator std::string() const;

  explicit operator Types() const;

  [[nodiscard]] std::string toString(bool printChildren) const override;

  bool operator==(const Datatype &rhs) const;

  bool operator!=(const Datatype &rhs) const;

  [[nodiscard]] Types getType() const;

  [[nodiscard]] bool isEncrypted() const;

  void setEncrypted(bool isEncrypted);

  void accept(Visitor &v) override;

  [[nodiscard]] nlohmann::json toJson() const override;

  std::vector<AbstractNode *> getChildren() override;
  void removeChildren() override;

};

#endif //AST_OPTIMIZER_INCLUDE_INCLUDE_UTILITIES_DATATYPE_H_

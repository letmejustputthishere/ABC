#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_IF_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_IF_H_

#include "AbstractStatement.h"
#include "AbstractExpr.h"
#include <string>

// Designed to be called only with T = (const) AbstractNode and I = (const) If
template<typename T, typename  I>
class IfIteratorImpl : public BaseIteratorImpl<T> {
 private:
  T *ifNode = nullptr;
  /// 0 = condition, 1 = thenBranch, 2 = elseBranch, 3 = "end"
  uint position = 0;

 public:
  IfIteratorImpl(T *ifNode, uint position) : ifNode(ifNode), position(position) {};

  std::unique_ptr<BaseIteratorImpl<T>> clone() override {
    return std::make_unique<IfIteratorImpl>(ifNode, position);
  }
  void increment() override {
    ++position;
  }

  bool equal(const BaseIteratorImpl<T> &other) override {
    if (auto otherIf = dynamic_cast<I &>(other)) {
      return ifNode->isEqual(otherIf) && position==otherIf.position;
    } else {
      return false;
    }
  }

  T &operator*() override {
    switch (position) {
      case 0: return *ifNode->getCondition();
      case 1: return *ifNode->getThenBranch();
      case 2: return *ifNode->getElseBranch();
        // calling dereference on higher elements is an error
    }
  }
};

class If : public AbstractStatement {
 private:
  AbstractExpr *condition;
  Block *thenBranch;
  Block *elseBranch;
 public:
  If(AbstractExpr *condition, AbstractStatement *thenBranch);

  If(AbstractExpr *condition, AbstractStatement *thenBranch, AbstractStatement *elseBranch);

  ~If() override;

  If *clone() const override;

  [[nodiscard]] json toJson() const override;

  void accept(Visitor &v) override;

  [[nodiscard]] std::string getNodeType() const override;

  [[nodiscard]] AbstractExpr *getCondition() const;

  [[nodiscard]] Block *getThenBranch() const;

  [[nodiscard]] Block *getElseBranch() const;

  void setCondition(AbstractExpr *newCondition);

  void setThenBranch(Block *newThenBranch);

  void setElseBranch(Block *newElseBranch);

  int getMaxNumberChildren() override;

  void setAttributes(AbstractExpr *condition, AbstractStatement *thenBranch, AbstractStatement *elseBranch);

  [[nodiscard]] std::string toString(bool printChildren) const override;

  std::vector<AbstractNode *> getChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
  void removeChildren() override;
  iterator begin() override;
  const_iterator begin() const override;
  iterator end() override;
  const_iterator end() const override;
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_IF_H_

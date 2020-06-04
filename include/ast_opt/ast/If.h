#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_IF_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_IF_H_

#include "AbstractStatement.h"
#include "AbstractExpression.h"
#include <string>

// Designed to be called only with T = (const) AbstractNode
template<typename T>
class IfIteratorImpl : public BaseIteratorImpl<T> {
 private:
  // Select const If / If depending on whether or not T is const/non-const
  typedef typename std::conditional<std::is_const<T>::value, const If, If>::type I;
  I &ifNode;
  /// 0 = condition, 1 = thenBranch, 2 = elseBranch, 3 = "end"
  uint position = 0;
 protected:
  T &getNode() override {
    return ifNode;
  };
  const T &getNode() const override {
    return ifNode;
  };
 public:
  IfIteratorImpl(I &ifNode, uint position) : ifNode(ifNode), position(position) {};

  std::unique_ptr<BaseIteratorImpl<T>> clone() override {
    return std::make_unique<IfIteratorImpl>(ifNode, position);
  }
  void increment() override {
    ++position;
  }

  bool equal(const BaseIteratorImpl<T> &other) override {
    if (ifNode==other.getNode()) {
      auto otherIfPtr = dynamic_cast<const IfIteratorImpl *>(&other);
      assert(otherIfPtr); // If the other node is an If, the Iterator must be an IfIterator, too
      return (position==otherIfPtr->position);
    } else {
      return false;
    }
  }

  T &operator*() override {
    switch (position) {
      case 0: return *ifNode.getCondition();
      case 1: return *ifNode.getThenBranch();
      case 2: return *ifNode.getElseBranch();
        // calling dereference on higher elements is an error
    }
  }
};

class If : public AbstractStatement {
 private:
  std::unique_ptr<AbstractExpression> condition = nullptr;
  std::unique_ptr<Block> thenBranch = nullptr;
  std::unique_ptr<Block> elseBranch = nullptr;
 public:
  ~If() override;

  If(std::unique_ptr<AbstractExpression>&& condition, std::unique_ptr<AbstractStatement>&& thenBranch, std::unique_ptr<
      AbstractStatement>&& elseBranch = nullptr);

  std::unique_ptr<If> clone() const;

  std::unique_ptr<AbstractNode> clone() const override;

  [[nodiscard]] const AbstractExpression * getCondition() const;

  [[nodiscard]] const Block * getThenBranch() const;

  [[nodiscard]] const Block * getElseBranch() const;

  void setCondition(std::unique_ptr<AbstractExpression> &&newCondition);

  void setThenBranch(std::unique_ptr<Block> &&newThenBranch);

  void setElseBranch(std::unique_ptr<Block> &&newElseBranch);




  int getMaxNumberChildren() override;
  std::vector<AbstractNode *> getChildren() override;
  std::vector<const AbstractNode *> getChildren() const override;
  void removeChildren() override;

  iterator begin() override;
  const_iterator begin() const override;
  iterator end() override;
  const_iterator end() const override;

  [[nodiscard]] std::string getNodeType() const override;
  [[nodiscard]] json toJson() const override;
  void accept(Visitor &v) override;

};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_IF_H_

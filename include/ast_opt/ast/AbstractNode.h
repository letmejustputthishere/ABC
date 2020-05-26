#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTNODE_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTNODE_H_

#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <nlohmann/json.hpp>
#include "ast_opt/visitor/Visitor.h"

class AbstractNode {
 protected:

  /// Stores the parent nodes of the current node.
  AbstractNode *parent = nullptr;

 public:
  /// Virtual Destructor, force class to be abstract
  virtual ~AbstractNode() = 0;

  /// Clones a node recursively, i.e., by including all of its children.
  /// \param keepOriginalUniqueNodeId Specifies whether to keep all of the unique node IDs of the original nodes.
  /// \return A clone of the node including clones of all of its children.
  [[nodiscard]] virtual AbstractNode *clone() const = 0;

  /// Casts a node to type T which must be the specific derived class of the node to cast successfully.
  /// \tparam T The derived class of the node object.
  /// \return A pointer to the casted object, or a std::logic_error if cast was unsuccessful.
  template<typename T>
  T *castTo() {
    if (auto castedNode = dynamic_cast<T *>(this)) {
      return castedNode;
    } else {
      std::stringstream outputMsg;
      outputMsg << "Cannot cast object of type AbstractNode to given class ";
      outputMsg << typeid(T).name() << ". ";
      outputMsg << "Because node (" << this->getUniqueNodeId() << ") is of type ";
      outputMsg << this->getNodeType() << ".";
      throw std::logic_error(outputMsg.str());
    }
  }

  /// Part of the visitor pattern.
  /// Must be overridden in derived classes and must call v.visit(node).
  /// This allows the correct overload for the derived class to be called in the visitor.
  /// \param v Visitor that offers a visit() method
  virtual void accept(Visitor &v) = 0;

  /** @defgroup children Methods for handling children
   *  @{
   */

  /// Indicates the number of children that are allowed for a specific node.
  /// For example, a arithmetic expression accepts exactly three attributes and hence also exactly three children:
  /// left operand, right operand, and operator.
  /// If the node does not implement support for child/parent relationships, getMaxNumberChildren() return 0.
  /// \return An integer indicating the number of allowed children for a specific node.
  virtual int getMaxNumberChildren();

  /// Returns a reference to the vector of children nodes.
  /// \return A reference to the vector of this node's children.
  [[nodiscard]] virtual const std::vector<AbstractNode *> &getChildren() const = 0;

  /// Returns a vector of pointers to children nodes but without those children that are nullptr.
  /// \return A vector of non-nullptr children.
  [[nodiscard]] std::vector<AbstractNode *> getChildrenNonNull() const;

  /// Returns all the descendants nodes of the current node, i.e., the children of the children and the children of
  /// their children et cetera.
  /// \return A list of descendant nodes.
  std::vector<AbstractNode *> getDescendants();

  /// Removes all children from this node. Note: Does not update the child's parent.
  virtual void removeChildren() = 0;

  /// Replaces a given child (originalChild) of this node by a new node (newChild) and updates the child and
  /// parent references of both nodes. This method does not invalidate iterators over the children
  /// \param originalChild The node to be replaced by newChild.
  /// \param newChild The node to be added at the same position as the original child was.
  virtual void replaceChild(AbstractNode *originalChild, AbstractNode *newChild);

  /// Returns the number of children nodes that are not null (nullptr).
  /// \return An integer indicating the number of non-nullptr children nodes.
  [[nodiscard]] int countChildrenNonNull() const;

  /** @} */ // End of children group

  /** @defgroup parents Methods for handling Parents
   *  @{
   */

  /// Returns all the ancestor nodes of the current node, i.e., the ancestors of this node, the ancestors of the
  /// ancestors et cetera.
  /// \return A list of ancestor nodes.
  std::vector<AbstractNode *> getAncestors();

  /// Replaces the current parent with the newParent
  /// \param newParent The new parent node
  /// throws std::logic_error if the node already has a parent
  void setParent(AbstractNode *newParent);

  /// Removes parent from this node  and also removes this node from parent's children
  void removeFromParent();

  /// Checks whether this node has a certain parent (parentNode).
  /// \param parentNode The node that is searched for in this node's parent list.
  /// \return True if this node has the given parentNode as parent, otherwise returns False.
  bool hasParent(AbstractNode *parentNode);

  /// Checks whether this node has a parent set
  /// \return True if this node has a parent, otherwise returns False.
  bool hasParent() const;

  /// Returns a pointer to the only parent node.
  /// \return A pointer to the node's parent node.
  AbstractNode *getParent();

  /// Returns a pointer to the only parent node.
  /// \return A pointer to the node's parent node.
  const AbstractNode *getParent() const;

  /** @} */ // End of parents group

  /** @defgroup output Methods for output
  *  @{
  */

  /// Get the nlohmann::json representation of the node including all of its children.
  /// \return nlohmann::json representation of the node
  [[nodiscard]] virtual nlohmann::json toJson() const;

  /// Returns a string representation of the node, if printChildren is True then calls toString for its children too.
  /// Hence, toString(false) is equivalent to printing a node's attributes only.
  /// \return A textual representation of the node.
  [[nodiscard]] virtual std::string toString(bool printChildren) const;

  /// Generates an output string to be used by the toString() method.
  /// \param printChildren Specifies whether to print details of this node only (False) or also its children (True).
  /// \param attributes The node's attributes to be printed (fields in the node's class).
  /// \return A string representation of the node.
  [[nodiscard]] std::string generateOutputString(bool printChildren, std::vector<std::string> attributes) const;

  /** @} */ // End of output group

  /** @defgroup nodeID Methods to support unique node ids
  *  @{
  */

 private:
  /// Stores the reserved node ID until the first call of getUniqueNodeId() at which the reserved ID is
  /// fetched and the node's ID is assigned (field uniqueNodeId) based on the node's name and this reserved ID.
  /// This is a workaround because getNodeType() is a virtual method that cannot be called from derived classes'
  /// constructor.
  int assignedNodeId{-1};

  /// A static ongoing counter that is incremented after creating a new AbstractNode object. The counter's value is used
  /// to build the unique node ID.
  static int nodeIdCounter;

  /// An identifier that is unique among all nodes during runtime.
  mutable std::string uniqueNodeId;

  /// Generates a new node ID in the form "<NodeTypeName>_nodeIdCounter" where <NodeTypeName> is the value obtained by
  /// getNodeType() and nodeIdCounter an ongoing counter of created AbstractNode objects.
  /// \return An unique node ID to be used as uniqueNodeId for the current node.
  std::string generateUniqueNodeId() const;

  /// Returns the current ID (integer) and increments the ID by one. The ID is an ongoing counter of created
  /// AbstractNode objects and is used to build an AbstractNode's unique ID (see getUniqueNodeId()).
  /// \return The current ID as integer.
  static int getAndIncrementNodeId();

 protected:
  /// Default Constructor, defines some default behavior for subclasses related to IDs
  AbstractNode();

  /// Sets the uniqueNodeId attribute. This attribute should be auto-generated by generateUniqueNodeId().
  /// \param newUniqueNodeId The new unique node's identifier.
  void setUniqueNodeId(const std::string &newUniqueNodeId);

 public:
  /// Returns the node's type, which is the name of the object in the AST. This method must be overridden by all classes
  /// that inherit from AbstractNode by their respective name (e.g., ArithmeticExp, Function, Variable).
  /// \return The name of the node type.
  [[nodiscard]] virtual std::string getNodeType() const = 0;

  /// Returns a node's unique ID, or generates it by calling generateUniqueNodeId() if the name was not defined yet.
  /// \return The node's name consisting of the node type and an ongoing number (e.g., Function_1).
  std::string getUniqueNodeId() const;

  /// Resets the static node ID counter that is used to build the unique node ID.
  /// This method is required to be public for testing.
  static void resetNodeIdCounter();

  /** @} */ // End of nodeID group
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_AST_ABSTRACTNODE_H_

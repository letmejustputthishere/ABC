#include <sstream>
#include <queue>
#include <set>
#include "ast_opt/ast/AbstractNode.h"

int AbstractNode::nodeIdCounter = 0;

std::string AbstractNode::generateUniqueNodeId() const {
  if (assignedNodeId==-1) {
    throw std::logic_error("Could not find any reserved ID for node. "
                           "Node constructor needs to reserve ID for node (see empty constructor).");
  }

  // build and return the node ID string
  std::stringstream ss;
  ss << getNodeType() << "_" << assignedNodeId;
  return ss.str();
}

AbstractNode::AbstractNode() {
  // save the ID reserved for this node but do not build the unique node ID yet as this virtual method must not be
  // called within the constructor
  assignedNodeId = getAndIncrementNodeId();
}

std::string AbstractNode::getUniqueNodeId() const {
  // if there is no ID defined yet, create and assign an ID
  if (uniqueNodeId.empty()) this->uniqueNodeId = this->generateUniqueNodeId();
  // otherwise just return the previously generated ID
  return uniqueNodeId;
}

int AbstractNode::getAndIncrementNodeId() {
  return nodeIdCounter++;
}

void AbstractNode::resetNodeIdCounter() {
  AbstractNode::nodeIdCounter = 0;
}

const std::vector<AbstractNode *> &AbstractNode::getChildren() const {
  return children;
}

std::vector<AbstractNode *> AbstractNode::getChildrenNonNull() const {
  std::vector<AbstractNode *> childrenFiltered;
  if (children.empty()) return childrenFiltered;
  std::copy_if(children.begin(), children.end(), std::back_inserter(childrenFiltered),
               [](AbstractNode *n) { return n!=nullptr; });
  return childrenFiltered;
}

std::vector<AbstractNode *> AbstractNode::getParentsNonNull() const {
  std::vector<AbstractNode *> parentsFiltered;
  std::copy_if(parents.begin(), parents.end(), std::back_inserter(parentsFiltered),
               [](AbstractNode *n) { return n!=nullptr; });
  return parentsFiltered;
}

void AbstractNode::addChild(AbstractNode *child, bool addBackReference) {
  addChildren({child});
}

void AbstractNode::addChildren(const std::vector<AbstractNode *> &childrenToAdd, AbstractNode *insertBeforeNode) {
  auto it = std::find(children.begin(), children.end(), insertBeforeNode);
  if (it==children.end()) {
    throw std::runtime_error("addChildren failed: Could not find node given as parameter insertBeforeNode "
                             "that is required to determine insert position.");
  }
  addChildren(childrenToAdd, it);
}

void AbstractNode::addChildren(const std::vector<AbstractNode *> &childrenToAdd,
                               std::vector<AbstractNode *>::const_iterator insertPosition) {
  auto allowsInfiniteNumberOfChildren = (getMaxNumberChildren()==-1);

  // check whether the number of children to be added does not exceed the number available children spots
  if (!allowsInfiniteNumberOfChildren && childrenToAdd.size() > (getMaxNumberChildren() - countChildrenNonNull())) {
    std::stringstream errMsg;
    errMsg << "AbstractNode " << getUniqueNodeId() << " of type " << getNodeType() << " does not allow more than ";
    errMsg << std::to_string(getMaxNumberChildren()) << " children!";
    throw std::invalid_argument(errMsg.str());
  }

  // check if prependChildren is supported
  if (!allowsInfiniteNumberOfChildren && insertPosition!=children.end()) {
    throw std::runtime_error("addChildren failed: Cannot add node at specific position as node only supports a limited "
                             "number of children -> must add child in next free child spot.");
  }

  // these actions are to be performed after a node was added to the list of children
  auto doInsertPostAction = [&](AbstractNode *childToAdd) {
    // add a back reference to the child as parent
    if (childToAdd!=nullptr) childToAdd->setParent(this);
  };

  // if this nodes accepts an infinite number of children, pre-filling the slots does not make any sense -> skip it
  if (getMaxNumberChildren()!=-1) {
    // fill remaining slots with nullptr values
    children.insert(children.end(), getMaxNumberChildren() - getChildren().size(), nullptr);
  }

  // add the children one-by-one by looking for free slots
  size_t childIdx = 0;
  size_t idx = 0;
  // add child in first empty spot
  while (idx < children.size() && childIdx < childrenToAdd.size()) {
    if (children.at(idx)==nullptr) {
      children.at(idx) = childrenToAdd.at(childIdx);// insert the new child
      doInsertPostAction(children.at(idx));
      childIdx++;
    }
    idx++;
  }
  if (childIdx!=childrenToAdd.size()) {
    if (allowsInfiniteNumberOfChildren) {
      // then add all remaining nodes in one batch to the children vector's end
      children.insert(insertPosition, childrenToAdd.begin() + childIdx, childrenToAdd.end());
      std::for_each(childrenToAdd.begin(), childrenToAdd.end(), doInsertPostAction);
    } else {
      throw std::logic_error("Cannot add one or multiple children to " + this->getUniqueNodeId()
                                 + " without overwriting an existing one. Consider removing an existing child first.");
    }
  }
}

void AbstractNode::addChildren(const std::vector<AbstractNode *> &childrenToAdd) {
  addChildren(childrenToAdd, children.end());
}

void AbstractNode::removeChild(AbstractNode *child, bool removeBackreference) {
  auto it = std::find(children.begin(), children.end(), child);
  if (it!=children.end()) {
    if (removeBackreference) {
      (*it)->removeParent(this);
    }
    // if the node supports an infinite number of children (getMaxNumberChildren() == -1), we can delete the node from
    // the children list, otherwise we just overwrite the slot with a nullptr
    if (this->getMaxNumberChildren()!=-1) {
      *it = nullptr;
    } else {
      children.erase(it);
    }
  }
}

void AbstractNode::isolateNode() {
  for (auto &p : getParentsNonNull()) p->removeChild(this, false);
  for (auto &c : getChildrenNonNull()) c->removeParent(this);
  removeChildren();
  removeParents();
}

void AbstractNode::setParent(AbstractNode *newParent) {
  parents.push_back(newParent);
  for (auto &p : getParentsNonNull()) {
    p->addChild(this, false);
  }
}

void AbstractNode::removeParent(AbstractNode *parentToBeRemoved) {
  auto it = std::find(parents.begin(), parents.end(), parentToBeRemoved);
  if (it!=parents.end()) {
    (*it)->removeChild(this, false);
    parents.erase(it);
  }
}

void AbstractNode::removeChildren() {
  children.clear();
}

void AbstractNode::removeParents() {
  parents.clear();
}

void to_json(json &j, const AbstractNode &n) {
  j = n.toJson();
}

json AbstractNode::toJson() const {
  return json({"type", "AbstractNode"});
}

std::string AbstractNode::generateOutputString(bool printChildren, std::vector<std::string> attributes) const {
  std::string indentationCharacter("\t");
  std::stringstream ss;
  // -- example output --
  // Function (computeX):
  //   ParameterList:
  //     FunctionParameter:
  //       Datatype (int, plaintext)
  //       Variable (x)
  ss << getNodeType();
  if (!attributes.empty()) {
    ss << " (";
    for (auto it = attributes.begin(); it!=attributes.end(); ++it) {
      ss << *it;
      if ((it + 1)!=attributes.end()) ss << ", ";
    }
    ss << ")";
  }
  if (printChildren && countChildrenNonNull() > 0) ss << ":";
  ss << std::endl;
  if (printChildren) {
    for (auto &child : getChildrenNonNull()) ss << indentationCharacter << child->toString(printChildren);
  }
  return ss.str();
}

void AbstractNode::setUniqueNodeId(const std::string &newUniqueNodeId) {
  uniqueNodeId = newUniqueNodeId;
}

std::vector<AbstractNode *> AbstractNode::getAncestors() {
  // use a set to avoid duplicates as there may be common ancestors between this node and any of the node's parents
  std::set<AbstractNode *> result;
  std::queue<AbstractNode *> processQueue{{this}};
  while (!processQueue.empty()) {
    auto curNode = processQueue.front();
    processQueue.pop();
    auto nextNode = curNode->getParent();
    result.insert(nextNode);
    processQueue.push(nextNode);

  }
  return std::vector<AbstractNode *>(result.begin(), result.end());
}

std::vector<AbstractNode *> AbstractNode::getDescendants() {
  // use a set to avoid duplicates as there may be common descendants between this node and any of the node's children
  std::set<AbstractNode *> result;
  std::queue<AbstractNode *> processQueue{{this}};
  while (!processQueue.empty()) {
    auto curNode = processQueue.front();
    processQueue.pop();
    for (auto &node : curNode->getChildrenNonNull()) {
      result.insert(node);
      processQueue.push(node);
    }
  }
  return std::vector<AbstractNode *>(result.begin(), result.end());
}

bool AbstractNode::hasParent(AbstractNode *parentNode) {
  return getParent()==parentNode;
}

bool AbstractNode::hasParent() {
  return getParent()!=nullptr;
}

int AbstractNode::countChildrenNonNull() const {
  return std::count_if(getChildren().begin(), getChildren().end(), [](AbstractNode *n) { return n!=nullptr; });
}

int AbstractNode::getMaxNumberChildren() {
  return 0;
}

AbstractNode *AbstractNode::getChildAtIndex(int idx) const {
  return getChildAtIndex(idx, false);
}

AbstractNode *AbstractNode::getChildAtIndex(int idx, bool isEdgeDirectionAware) const {
  try {
    return (isEdgeDirectionAware && isReversed) ? parents.at(idx) : children.at(idx);
  } catch (std::out_of_range const &e) {
    return nullptr;
  }
}

AbstractNode::~AbstractNode() = default;

AbstractNode *AbstractNode::cloneFlat() {
  throw std::runtime_error("Cannot clone an AbstractNode. Use the overridden cloneFlat instead.");
}

void AbstractNode::replaceChild(AbstractNode *originalChild, AbstractNode *newChild) {
  auto pos = std::find(children.begin(), children.end(), originalChild);
  if (pos==children.end()) {
    throw std::runtime_error("Could not execute AbstractNode::replaceChildren because the node to be replaced could "
                             "not be found in the children vector!");
  }
  children[std::distance(children.begin(), pos)] = newChild;

  // remove edge: originalChild -> currentNode
  originalChild->removeParent(this);

  // add edges: newChildToBeAdded -> currentNode but before detach any existing parents from this child node
  if (newChild!=nullptr) {
    newChild->removeFromParents();
    newChild->setParent(this);
  }
}

AbstractNode *AbstractNode::removeFromParents(bool removeParentBackreference) {
  for (auto &p : getParentsNonNull()) {
    p->removeChild(this, removeParentBackreference);
  }
  return this;
}

std::string AbstractNode::toString(bool) const {
  throw std::runtime_error("toString not implemented for class " + getNodeType() + ".");
}

AbstractNode *AbstractNode::getParent() {
  auto parentsVector = getParentsNonNull();
  if (parentsVector.size() > 1) {
    throw std::logic_error("AbstractNode::getParent() failed because node has more than one parent!");
  } else if (parentsVector.empty()) {
    throw std::logic_error("AbstractNode::getParent() failed because node does not have a parent");
  }
  return this->getParent();
}

void AbstractNode::updateClone(bool keepOriginalUniqueNodeId, const AbstractNode *originalNode) {
  if (keepOriginalUniqueNodeId) setUniqueNodeId(originalNode->getUniqueNodeId());
}

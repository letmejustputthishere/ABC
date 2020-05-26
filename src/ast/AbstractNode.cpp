#include <sstream>
#include <queue>
#include <set>
#include "ast_opt/ast/AbstractNode.h"

using json = nlohmann::json;

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

std::vector<const AbstractNode *> AbstractNode::getChildrenNonNull() const {
  std::vector<const AbstractNode *> childrenFiltered;
  auto children = getChildren();
  if (children.empty()) return childrenFiltered;
  std::copy_if(children.begin(), children.end(), std::back_inserter(childrenFiltered),
               [](const AbstractNode *n) { return n!=nullptr; });
  return childrenFiltered;
}

int AbstractNode::countChildrenNonNull() const {
  return std::count_if(getChildren().begin(), getChildren().end(), [](const AbstractNode *n) { return n!=nullptr; });
}

int AbstractNode::getMaxNumberChildren() {
  return 0;
}

void AbstractNode::replaceChild(AbstractNode *originalChild, AbstractNode *newChild) {
  //TODO: Make this more efficient
  auto children = getChildren();
  auto pos = std::find(children.begin(), children.end(), originalChild);
  if (pos==children.end()) {
    throw std::runtime_error("Could not execute AbstractNode::replaceChildren because the node to be replaced could "
                             "not be found in the children vector!");
  }
  children[std::distance(children.begin(), pos)] = newChild;

  // remove edge: originalChild -> currentNode
  originalChild->removeFromParent();

  // add edges: newChildToBeAdded -> currentNode but before detach any existing parents from this child node
  if (newChild!=nullptr) {
    newChild->removeFromParent();
    newChild->setParent(this);
  }
};

bool AbstractNode::hasParent(AbstractNode *parentNode) {
  return getParent()==parentNode;
}

bool AbstractNode::hasParent() const {
  return getParent()!=nullptr;
}

AbstractNode *AbstractNode::getParent() {
  return parent;
}

const AbstractNode *AbstractNode::getParent() const {
  return parent;
}

void AbstractNode::setParent(AbstractNode *newParent) {
  if (parent) {
    throw std::logic_error("Cannot overwrite parent.");
  } else {
    parent = newParent;
  }
}

void AbstractNode::removeFromParent() {
  if (!parent) {
    throw std::logic_error("Cannot remove node from parent, since node does not have a parent!");
  } else {
    throw std::runtime_error("NOT IMPLEMENTED");
  }
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
    for (auto &node : curNode->getChildren()) {
      if (node) {
        result.insert(node);
        processQueue.push(node);
      }
    }
  }
  return std::vector<AbstractNode *>(result.begin(), result.end());
}

std::vector<const AbstractNode *> AbstractNode::getDescendants() const {
  // use a set to avoid duplicates as there may be common descendants between this node and any of the node's children
  std::set<const AbstractNode *> result;
  std::queue<const AbstractNode *> processQueue{{this}};
  while (!processQueue.empty()) {
    auto curNode = processQueue.front();
    processQueue.pop();
    for (auto &node : curNode->getChildren()) {
      if (node) {
        result.insert(node);
        processQueue.push(node);
      }
    }
  }
  return std::vector<const AbstractNode *>(result.begin(), result.end());
}

AbstractNode::~AbstractNode() = default;

std::string AbstractNode::toString(bool) const {
  throw std::runtime_error("toString not implemented for class " + getNodeType() + ".");
}
std::vector<const AbstractNode *> AbstractNode::getChildren() const {
  //TODO?
  return {};
}


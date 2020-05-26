#include "ast_opt/utilities/DotPrinter.h"
#include "ast_opt/ast/VarDecl.h"
#include "ast_opt/ast/VarAssignm.h"

DotPrinter::DotPrinter() : mdc(nullptr) {}

DotPrinter &DotPrinter::setShowMultDepth(bool show_mult_depth) {
  showMultDepth = show_mult_depth;
  if (show_mult_depth && mdc==nullptr) {
    throw std::logic_error(
        "Printing the multiplicative depth for nodes requires providing a MultiplicativeDepthCalculator instance "
        "using setMultiplicativeDepthsCalculator(...) prior calling setShowMultDepth(true).");
  }
  return *this;
}

DotPrinter &DotPrinter::setIndentationCharacter(const std::string &indentation_character) {
  indentationCharacter = indentation_character;
  return *this;
}

DotPrinter &DotPrinter::setOutputStream(std::ostream &stream) {
  outputStream = &stream;
  return *this;
}

DotPrinter &
DotPrinter::setMultiplicativeDepthsCalculator(MultiplicativeDepthCalculator &multiplicativeDepthCalculator) {
  mdc = &multiplicativeDepthCalculator;
  return *this;
}

std::string DotPrinter::getDotFormattedString(const AbstractNode *n) {
  std::stringstream finalString;

  auto vec = n->getChildrenNonNull();

  // define criteria when to print node details
  auto printNodeDetailsCriterion = (vec.empty()    // if node is a tree leaf
      || dynamic_cast<const VarDecl *>(n)!=nullptr       // if node is a VarDecl (needed for the variable identifier)
      || dynamic_cast<const VarAssignm *>(n)!=nullptr);  // if node is a VarAssignm (needed for the variable identifier)
  finalString << DotVertex(n, this->showMultDepth, this->mdc, printNodeDetailsCriterion)
      .buildVertexString(this->indentationCharacter);

  // only print edges if there are any edges at all
  if (vec.empty()) return finalString.str();

  // otherwise also generate string for edge and return both
  finalString << DotEdge(n).buildEdgeString(this->indentationCharacter);
  return finalString.str();
}

void DotPrinter::printAsDotFormattedGraph(Ast &ast) {
  *outputStream << "digraph D {" << std::endl;
  std::deque<std::pair<const AbstractNode *, int>> q;
  q.emplace_back(ast.getRootNode(), 1);
  while (!q.empty()) {
    auto curNode = q.front().first;
    auto il = q.front().second;
    q.pop_front();
    *outputStream << getDotFormattedString(curNode);
    auto nodes = (ast.isReversed()) ? std::vector<const AbstractNode *>(1, curNode->getParent()) : curNode->getChildrenNonNull();
    for (auto &n : nodes) q.emplace_front(n, il + 1);
  }
  *outputStream << "}" << std::endl;
}

void DotPrinter::printAllReachableNodes(const AbstractNode *pNode) {
  std::set<const AbstractNode *> printedNodes;
  std::queue<const AbstractNode *> q{{pNode}};
  while (!q.empty()) {
    auto curNode = q.front();
    q.pop();
    if (printedNodes.count(curNode)==0) {
      *outputStream << DotPrinter::getDotFormattedString(curNode);
      for (auto &c : curNode->getChildrenNonNull()) { q.push(c); }
      q.push(curNode->getParent());
      printedNodes.insert(curNode);
    }
  }
}

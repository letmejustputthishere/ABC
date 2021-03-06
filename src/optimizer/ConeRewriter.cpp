#include <set>
#include <queue>
#include <utility>
#include "ast_opt/optimizer/ConeRewriter.h"
#include "ast_opt/ast/LogicalExpr.h"
#include "ast_opt/ast/Return.h"
#include "ast_opt/ast/Function.h"
#include "ast_opt/utilities/NodeUtils.h"

ConeRewriter::ConeRewriter(Ast *ast)
    : ast(ast), mdc(MultiplicativeDepthCalculator{*ast}) {
}

ConeRewriter::ConeRewriter(Ast *ast, MultiplicativeDepthCalculator &mdc)
    : ast(ast), mdc(mdc) {}

ConeRewriter::~ConeRewriter() = default;

// ------------------------------------–
// Algorithms presented in the paper
// ------------------------------------–

Ast &ConeRewriter::applyConeRewriting() {
  // Ensure that we have a circuit of supported nodes only
  if (!ast->isValidCircuit()) {
    throw std::invalid_argument(
        "Cannot perform Cone Rewriting on AST because AST consists of AbstractNode types that are not circuit-compatible. "
        "These nodes do not define the required child/parent relationship of class AbstractNode.");
  }
  return applyMinMultDepthHeuristic();
}

Ast &ConeRewriter::applyMinMultDepthHeuristic() {
  // create a deep copy of the input circuit but keep the original unique node IDs, otherwise the matching of the
  // precomputed multiplicative depth maps (which is based on the unique node ID of the original AST) will fail and
  // recomputation will be required.
  // cOut is the 'safe copy' that contains the best circuit obtained by rewritings so far.
  Ast *cOut = new Ast(*ast, true);

  // compute reducible cones set
  std::vector<AbstractNode *> deltaMin = ConeRewriter::computeReducibleCones();

  // repeat rewriting as long as there are any reducible cones.
  // optimizedCkt will contain optimized circuit with all rewritten cones
  while (!deltaMin.empty()) {
    // rewrite all cones from deltaMin in the circuit 'ast' that contains all positive transformations so far
    // (positive in the sense that the transformation reduced the multiplicative depth)
    rewriteCones(*ast, deltaMin);
    MultiplicativeDepthCalculator newMdc(*ast);
    // if depth of cone-rewritten circuit is smaller -> this is our new circuit that is used to explore more cones
    if (getMaxMultDepth(*cOut) > newMdc.getMaximumMultiplicativeDepth()) {
      // store pointer to this optimized circuit into the attribute 'ast' by creating a copy
      // perform next rewritings (as previously) on object ast
      delete cOut;
      cOut = new Ast(*ast, true);
      mdc = newMdc;
    }

    // recompute reducible cones to proceed rewriting
    // NOTE: according to the paper, this is not included into the If-branch.
    deltaMin = ConeRewriter::computeReducibleCones();
  }

  // return multiplicative depth-optimized circuit
  return *cOut;
}

std::vector<AbstractNode *> ConeRewriter::computeReducibleCones() {
  // We need to reverse the original tree's edges to be conform to the paper. This, however, is only required to perform
  // cone selection.
  if (!ast->isReversed()) ast->reverseEdges();

  // delta: set of all reducible cones
  std::vector<AbstractNode *> delta = getReducibleCones();

  // cAndCkt: C^{AND} circuit consisting of critical AND-nodes that are connected if there is a multiplicative depth-2
  // path in between two of those nodes in the initial circuit
  // each cone δ ∈ Δ has an unique terminal AND node in C^{AND}
  std::vector<AbstractNode *> cAndCkt = ConeRewriter::getAndCriticalCircuit(delta);

  // deltaMin: the minimum set of reducible cones
  std::vector<AbstractNode *> deltaMin = ConeRewriter::selectCones(cAndCkt);

  // make sure that AST is reversed back if it was reversed at the beginning of this function
  if (ast->isReversed()) ast->reverseEdges();

  return deltaMin;
}

std::vector<AbstractNode *> ConeRewriter::getReducibleCones() {
  AbstractNode *startNode = nullptr;
  auto rootNode = ast->getRootNode();

  auto getNthAncestorLogicalExpr = [&](int N) -> AbstractNode * {
    // find first LogicalExpr in AST
    std::pair<AbstractNode *, int> candidate(nullptr, 0);
    std::queue<AbstractNode *> q{{rootNode}};
    while (!q.empty()) {
      auto curNode = q.front();
      q.pop();
      if (dynamic_cast<LogicalExpr *>(curNode)) {
        candidate = std::make_pair(curNode, candidate.second + 1);
        if (candidate.first!=nullptr && candidate.second==N)
          return candidate.first;
      }
      // add all parent nodes
      std::for_each(curNode->getParents().begin(), curNode->getParents().end(), [&](AbstractNode *n) { q.push(n); });
    }
    return nullptr;
  };

  // check if v (rootNode) is a LogicalExpr, otherwise find first (N=1) LogicalExpr by its traversing parents
  if (dynamic_cast<LogicalExpr *>(rootNode)==nullptr) {
    startNode = getNthAncestorLogicalExpr(1);
    if (!startNode) throw std::logic_error("AST does not contain any LogicalExpr!");
  }

  // compute minDepth required by getReducibleCones algorithm
  int minDepth = computeMinDepth(startNode);

  // v has no non-critical input node p -> return empty set
  if (minDepth==-1) return std::vector<AbstractNode *>();

  return getReducibleCones(startNode, minDepth);
}

// ------------------------------------–
// Internal (non-universal) helper methods
// ------------------------------------–

int ConeRewriter::computeMinDepth(AbstractNode *v) {
  // find a non-critical input node p of v
  auto isNoOperatorNode = [](AbstractNode *n) { return (dynamic_cast<Operator *>(n)==nullptr); };
  for (auto &p : v->getParentsNonNull()) {
    // exclude Operator nodes as they do not have any parent and are not modeled as node in the paper
    if (isNoOperatorNode(p) && !isCriticalNode(p)) {
      // set minMultDepth as l(p)+2 and call getReducibleCones
      //return mdc.getMultDepthL(p) + 2;
      // According to the paper (see p. 9, §2.4) minMultDepth = l(p)+1 is used but it does not return any result:
      return mdc.getMultDepthL(p) + 1;
    }
  }
  // return -1 (error) if node v has no non-critical input node
  return -1;
}

std::vector<AbstractNode *> ConeRewriter::getReducibleCones(AbstractNode *v, int minDepth) {
  // return empty set if minDepth is reached
  if (mdc.getMultDepthL(v)==minDepth) return std::vector<AbstractNode *>();

  // get predecessor nodes on critical path, i.e., { p ∈ pred(v) | l(p) = l(v) - d(v) }
  std::vector<AbstractNode *> *P = getPredecessorOnCriticalPath(v);

  // return v if at least one predecessor of v is non-critical and v is an AND-gate
  auto logicalExp = v->castTo<LogicalExpr>();
  if (P->size() < 2 && logicalExp!=nullptr && logicalExp->getOperator()->equals(LogCompOp::LOGICAL_AND)) {
    // return set consisting of start node v only
    return std::vector<AbstractNode *>{v};
  }

  // determine reducible input cones
  std::vector<std::vector<AbstractNode *>> deltaR;
  std::vector<AbstractNode *> delta;
  for (auto &p : *P) {
    std::vector<AbstractNode *> intermedResult = getReducibleCones(p, computeMinDepth(p));
    if (!intermedResult.empty()) deltaR.push_back(intermedResult);
  }

  // return empty set if either one of the following is true:
  // a. v is not a LogicalExpr
  // b. v is a LogicalExpr but not an AND- or XOR-gate
  // b. v is an AND-gate and deltaR is empty
  // c. v is a XOR-gate and size of deltaR does not equal size of P
  if (logicalExp==nullptr ||
      !(logicalExp->getOperator()->equals(LogCompOp::LOGICAL_AND)
          || logicalExp->getOperator()->equals(LogCompOp::LOGICAL_XOR)) ||
      (logicalExp->getOperator()->equals(LogCompOp::LOGICAL_AND) && deltaR.empty()) ||
      (logicalExp->getOperator()->equals(LogCompOp::LOGICAL_XOR) && deltaR.size()!=P->size())) {
    return std::vector<AbstractNode *>();
  }

  if (logicalExp->getOperator()->equals(LogCompOp::LOGICAL_AND)) {
    // both cones must be reducible because deltaR is non-empty -> pick a random one, and assign to delta
    delta = *select_randomly(deltaR.begin(), deltaR.end());
  } else if (logicalExp->getOperator()->equals(LogCompOp::LOGICAL_XOR)) {
    // critical cones must be reducible because size of deltaR equals size of P
    // flatten vector deltaR consisting of sets generated each by getReducibleCones
    std::vector<AbstractNode *> flattenedDeltaR;
    flattenVectors(flattenedDeltaR, deltaR);
    // add all elements of flattened deltaR to delta
    addElements(delta, flattenedDeltaR);
  }

  // return 𝛅 ⋃ {v}
  delta.push_back(v);
  return delta;
}

int ConeRewriter::getMaxMultDepth(Ast &inputAst) {
  std::queue<AbstractNode *> nodesToCheck;
  nodesToCheck.push(inputAst.getRootNode());
  int highestDepth = 0;
  while (!nodesToCheck.empty()) {
    auto curNode = nodesToCheck.front();
    nodesToCheck.pop();
    highestDepth = std::max(highestDepth, mdc.getMultDepthL(curNode));
    auto vec = inputAst.isReversed() ? curNode->getParents() : curNode->getChildren();
    std::for_each(vec.begin(), vec.end(), [&](AbstractNode *n) {
      nodesToCheck.push(n);
    });
  }
  return highestDepth;
}

void ConeRewriter::addElements(std::vector<AbstractNode *> &result, std::vector<AbstractNode *> newElements) {
  result.reserve(result.size() + newElements.size());
  result.insert(result.end(), newElements.begin(), newElements.end());
}

void ConeRewriter::flattenVectors(std::vector<AbstractNode *> &resultVector,
                                  std::vector<std::vector<AbstractNode *>> vectorOfVectors) {
  std::set<AbstractNode *> res;
  std::for_each(vectorOfVectors.begin(), vectorOfVectors.end(), [&](std::vector<AbstractNode *> rVec) {
    res.insert(rVec.begin(), rVec.end());
  });
  resultVector.assign(res.begin(), res.end());
}

std::vector<AbstractNode *> *ConeRewriter::getPredecessorOnCriticalPath(AbstractNode *v) {
  // P <- { p ∈ pred(v) | l(p) = l(v) - d(v) }
  auto result = new std::vector<AbstractNode *>();
  int criterion = mdc.getMultDepthL(v) - mdc.depthValue(v);
  for (auto &p : v->getParentsNonNull()) {
    if (mdc.getMultDepthL(p)==criterion) result->push_back(p);
  }
  return result;
}

void ConeRewriter::reverseEdges(const std::vector<AbstractNode *> &nodes) {
  for (AbstractNode *n : nodes) n->swapChildrenParents();
}

/// Method used for debugging purposes only
void ConeRewriter::getReducibleConesForEveryPossibleStartingNode(Ast &inputAst) {
  for (AbstractNode *n : inputAst.getRootNode()->getAncestors()) {
    int minDepth = computeMinDepth(n);
    if (minDepth==-1) continue;
    std::vector<AbstractNode *> delta = getReducibleCones(n, minDepth);

    // cAndCkt: C^{AND} circuit consisting of critical AND-nodes that are connected if there is a multiplicative depth-2
    // path in between two of those nodes in the initial circuit
    // each cone δ ∈ Δ has an unique terminal AND node in C^{AND}
    std::vector<AbstractNode *> cAndCkt = ConeRewriter::getAndCriticalCircuit(delta);

    // deltaMin: the minimum set of reducible cones
    std::vector<AbstractNode *> deltaMin = ConeRewriter::selectCones(cAndCkt);
  }
}

bool ConeRewriter::isCriticalNode(AbstractNode *n) {
  int l = mdc.getMultDepthL(n);
  int r = mdc.getReverseMultDepthR(n);
  return (mdc.getMaximumMultiplicativeDepth()==l + r);
}

std::vector<AbstractNode *> ConeRewriter::getAndCriticalCircuit(std::vector<AbstractNode *> delta) {
  // remove non-AND nodes from delta (note: delta is passed as copy-by-value) as delta may also include XOR nodes
  delta.erase(remove_if(delta.begin(), delta.end(), [](AbstractNode *d) {
    auto lexp = dynamic_cast<LogicalExpr *>(d);
    return (lexp==nullptr || !lexp->getOperator()->equals(LogCompOp::LOGICAL_AND));
  }), delta.end());

  // duplicate critical nodes to create new circuit C_{AND} as we do not want to modify the original circuit
  std::unordered_map<std::string, AbstractNode *> cAndMap;
  std::vector<AbstractNode *> cAndResultCkt;
  for (auto &v : delta) {
    // note that cloneFlat() does not copy the links to parents and children
    auto clonedNode = v->cloneFlat();
    // a back-link to the node in the original circuit
    underlying_nodes.insert(std::make_pair<std::string, AbstractNode *>(v->getUniqueNodeId(), &*v));
    cAndMap.emplace(v->getUniqueNodeId(), clonedNode);
    cAndResultCkt.push_back(clonedNode);
  }

  // in case that there are less than two nodes, we can not connect any two nodes
  if (delta.size() < 2) return cAndResultCkt;

  // check if there are depth-2 critical paths in between critical nodes in the original ckt
  for (auto &v : delta) {
    std::queue<AbstractNode *> q{{v}};
    while (!q.empty()) {
      auto curNode = q.front();
      q.pop();
      for (auto &child : curNode->getChildren()) {
        auto childLexp = dynamic_cast<LogicalExpr *>(child);
        // if the child is a LogicalExpr of type AND-gate
        if (childLexp!=nullptr && childLexp->getOperator()->equals(LogCompOp::LOGICAL_AND)) {
          // check if this child is a critical node, if yes: connect both nodes
          if (std::find(delta.begin(), delta.end(), childLexp)!=delta.end()) {
            AbstractNode *copiedV = cAndMap[v->getUniqueNodeId()];
            AbstractNode *copiedChild = cAndMap[child->getUniqueNodeId()];
            copiedV->addChild(copiedChild, false);
            copiedChild->addParent(copiedV, false);
          }
        } else {  // continue if child is not a LogicalExpr --> node does not influence the mult. depth
          q.push(child);
        }
      }
    }
  }
  return cAndResultCkt;
}

/// This method implements Kahn's algorithm as described at wikipedia,
/// see https://en.wikipedia.org/wiki/Topological_sorting#CITEREFKahn1962
std::vector<AbstractNode *> ConeRewriter::sortTopologically(const std::vector<AbstractNode *> &nodes) {
  std::vector<AbstractNode *> L;
  std::map<AbstractNode *, int> numEdgesDeleted;

  // S <- nodes without an incoming edge
  std::vector<AbstractNode *> S;
  std::for_each(nodes.begin(), nodes.end(), [&](AbstractNode *n) {
    if (n->getParents().empty()) S.push_back(n);
  });

  while (!S.empty()) {
    auto n = S.back();
    S.pop_back();
    L.push_back(n);
    for (auto &m : n->getChildrenNonNull()) {
      numEdgesDeleted[m] += 1; // emulates removing edge from the graph
      if (m->getParents().size()==numEdgesDeleted[m]) S.push_back(m);
    }
  }
  return L;
}

std::vector<AbstractNode *> ConeRewriter::selectCones(std::vector<AbstractNode *> cAndCkt) {
  /// Helper function that computes the flow of all nodes in the given circuit ckt
  auto computeFlow = [](std::vector<AbstractNode *> &ckt) {
    std::map<AbstractNode *, float> computedFlow;
    std::map<std::pair<AbstractNode *, AbstractNode *>, float> edgeFlows;
    auto topologicalOrder = ConeRewriter::sortTopologically(ckt);
    for (AbstractNode *v : topologicalOrder) {
      if (v->getParentsNonNull().empty()) {  // if v is input
        // trivial flow of 1
        computedFlow[v] = 1;
      } else {  // if v is intermediate node
        // compute flow by accumulating flows of incoming edges (u,v) where u ∈ pred(v)
        auto predecessorsOfV = v->getParentsNonNull();
        float flow = 0.0f;
        std::for_each(predecessorsOfV.begin(), predecessorsOfV.end(), [&](AbstractNode *u) {
          flow += edgeFlows[std::pair(u, v)];
        });
        computedFlow[v] = flow;
      }
      // for all successors u: define edge flow
      for (auto &u : v->getChildrenNonNull()) {
        edgeFlows[std::make_pair(v, u)] = computedFlow[v]/v->getChildrenNonNull().size();
      }
    }
    return computedFlow;
  };

  // ----------------------------

  std::vector<AbstractNode *> deltaMin;
  while (!cAndCkt.empty()) {
    // compute all node flows f^{+}(v) in cAndCkt
    std::map<AbstractNode *, float> nodeFlows = computeFlow(cAndCkt);

    // reverse circuit edges and compute all ascending node flows f^{-}(v)
    ConeRewriter::reverseEdges(cAndCkt);
    std::map<AbstractNode *, float> nodeFlowsAscending = computeFlow(cAndCkt);

    // compute f(v) for all nodes
    std::map<AbstractNode *, float> flows;
    for (auto &n : cAndCkt) flows[n] = nodeFlowsAscending[n]*nodeFlows[n];

    // find the node with the largest flow u
    AbstractNode *u = std::max_element(
        flows.begin(),
        flows.end(),
        [](const std::pair<AbstractNode *, float> &p1, const std::pair<AbstractNode *, float> &p2) {
          return p1.second < p2.second;
        })->first;

    // remove node u
    // - remove any edges pointing to node u as child
    for (auto &p : u->getParentsNonNull()) p->removeChild(u, false);

    // - remove any edges pointing to node u as parent
    for (auto &p : u->getChildrenNonNull()) p->removeParent(u, false);

    // - remove node u from cAndCkt
    cAndCkt.erase(std::remove(cAndCkt.begin(), cAndCkt.end(), u), cAndCkt.end());

    // add critical cone ending at node u to deltaMin
    deltaMin.push_back(u);
  }

  return deltaMin;
}

AbstractNode *ConeRewriter::getBFSLastNonCriticalLeafNode() {
  AbstractNode *candidateNode = nullptr;
  std::queue<AbstractNode *> q{{ast->getRootNode()}};
  while (!q.empty()) {
    auto curNode = q.front();
    q.pop();
    for (auto &p : curNode->getParentsNonNull()) {
      // remember found non-critical leaf node
      if (p->getParents().empty() && !isCriticalNode(p)) candidateNode = p;
      // enqueue the parent nodes and continue
      q.push(p);
    }
  }
  // returns the non-critical leaf node with the highest tree-depth
  return candidateNode;
}

void ConeRewriter::rewriteCones(Ast &astToRewrite, const std::vector<AbstractNode *> &coneEndNodes) {
  // reverse back the AST that has been reversed previously to facilitate the cone selection algorithms.
  // as we need to, however, modify the graph now, we need the "original" order again to not break anything
  // no need to recompute multiplicative depths after reversing edges as it does not change
  if (astToRewrite.isReversed()) astToRewrite.reverseEdges();

  // Assumption: δ ∈ coneEndNodes represents a cone that ends at node δ
  for (auto coneEnd : coneEndNodes) {
    // we need to get the node in the underlying circuit as C^{AND} only contains a limited subset of nodes
    coneEnd = underlying_nodes.find(coneEnd->getUniqueNodeId())->second;
    // determine bounds of the cone
    // -- upper bound: parent node of cone end
    auto rNode = coneEnd->getOnlyParent();

    // U_y, the multi-input XOR, is represented as multiple XOR nodes in our tree:
    // coneEndNodes [v_1, ..., v_n] --> xorN --> xorN-1 ---> xorN-2 ---> ... ---> xor1 --> sNode v_t.
    // We denote xorN as xorEndNode and xor1 as xorStartNode. We know that xorStartNode must be the first node in the
    // cone, i.e., the first child of the cone's end node.
    auto *coneEndAsLogicalExpr = coneEnd->castTo<LogicalExpr>();
    auto[xorStartNode, a_t] = getCriticalAndNonCriticalInput(coneEndAsLogicalExpr);
    // we "cut-off" parent edge between coneEnd and a_t in order to reconnect a_t later (but we keep a_t's children!)
    a_t->removeParent(coneEnd, false);
    coneEnd->removeChild(a_t, false);

    // -- lower bound: first AND node while following critical path
    // find the ancestors of δ that are LogicalExpr --> start nodes (v_1, ..., v_n) of cone
    std::vector<AbstractNode *> coneStartNodes;
    std::queue<AbstractNode *> q{{coneEnd}};
    while (!q.empty()) {
      auto curNode = q.front();
      q.pop();
      auto curNodeLexp = dynamic_cast<LogicalExpr *>(curNode);
      // if curNode is not the cone end node and a logical-AND expression, we found one of the end nodes
      // in that case we can stop exploring this path any further
      if (curNode!=coneEnd && curNodeLexp!=nullptr && curNodeLexp->getOperator()->equals(LogCompOp::LOGICAL_AND)) {
        coneStartNodes.push_back(curNode);
      } else {  // otherwise we need to continue search by following the critical path
        // add parent nodes of current nodes -> continue BFS traversal
        for (auto &child : curNode->getChildrenNonNull()) {
//        for (auto &child : curNode->getParentsNonNull()) {
          if (isCriticalNode(child)) { q.push(child); }
        }
      }
    }

    std::vector<AbstractNode *> finalXorInputs;
    // It should not make a difference which of the cone start nodes we take - all of them should have the same parent.
    AbstractNode *xorEndNode = coneStartNodes.front()->getOnlyParent();
    for (auto &startNode : coneStartNodes) assert(startNode->getOnlyParent()==xorEndNode);

    // check whether we need to handle non-critical inputs y_1, ..., y_m
    if (dynamic_cast<LogicalExpr *>(xorEndNode)->getOperator()->equals(LogCompOp::LOGICAL_AND) && xorEndNode==coneEnd) {
      // if there are no non-critical inputs y_1, ..., y_m then the cone's end and cone's start are both connected with
      // each other.
      // remove the edge between the start nodes and the end node
      for (auto &node : coneStartNodes) {
        coneEnd->removeChild(node, true);
      }
    } else {
      // collect all non-critical inputs y_1, ..., y_m in between xorStartNode up to xorEndNode
      std::vector<AbstractNode *> inputsY1ToYm;
      auto currentNode = xorStartNode;
      while (true) {
        auto *currentNodeAsLogicalExpr = dynamic_cast<LogicalExpr *>(currentNode);
        if (currentNode==nullptr)
          throw std::logic_error(
              "AbstractNode between cone end and cone start node is expected to be a logical expression!");
        auto[critIn, nonCritIn] = getCriticalAndNonCriticalInput(currentNodeAsLogicalExpr);
        currentNode->removeChild(nonCritIn, false);
        nonCritIn->removeParent(currentNode, false);
        inputsY1ToYm.push_back(nonCritIn);
        currentNode = critIn;
        // termination criteria (1): we reached the end of the XOR chain
        if (currentNode==xorEndNode) {
          auto *xorEndNodeAsLogicalExpr = dynamic_cast<LogicalExpr *>(currentNode);
          // if only one of both is critical, we need to enqueue the non-critical of them.
          // exactly one of both inputs is non-critical
          //   <=> left is critical XOR right is critical
          //   <=> (left is critical) unequal (right is critical)
          if (isCriticalNode(xorEndNodeAsLogicalExpr->getLeft())
              !=isCriticalNode(xorEndNodeAsLogicalExpr->getRight())) {
            // then we need to collect this non-critical input
            auto nonCriticalInput = getCriticalAndNonCriticalInput(xorEndNodeAsLogicalExpr).second;
            inputsY1ToYm.push_back(nonCriticalInput);
          }
          // stop traversing further as we reached the xorEndNode
          break;
        }
        // termination criteria (2): we already reached a cone start node
        if (std::find(coneStartNodes.begin(), coneStartNodes.end(), currentNode)!=coneStartNodes.end()) {
          break;
        }
      }

      // XorEndNode: remove incoming edges from nodes v_1, ..., v_n
      for (auto &p : xorEndNode->getChildrenNonNull()) {
        // keep the operator node -> only remove those nodes that are no operators
        if (dynamic_cast<Operator *>(p)==nullptr) xorEndNode->removeChild(p, true);
      }

      // XorStartNode: remove incoming edge from xorStartNode to v_t
      for (auto &c : xorStartNode->getParentsNonNull()) {
        c->removeChild(xorStartNode, true);
      }

      // check that at least one y_i is present
      if (inputsY1ToYm.empty())
        throw std::logic_error("Unexpected number (0) of non-critical inputs y_1, ..., y_m.");

      // build new node u_y that is connected to inputs y_1, ..., y_m and a_t
      AbstractExpr *u_y_rightOperand = nullptr;
      if (inputsY1ToYm.size()==1) {
        // if y_1 only exists: connect input y_1 directly to u_y -> trivial case
        u_y_rightOperand = inputsY1ToYm.front()->castTo<AbstractExpr>();
      } else if (inputsY1ToYm.size() > 1) {  // otherwise there are inputs y_1, y_2, ..., y_m
        // otherwise build XOR chain of inputs and connect last one as input of u_y
        std::vector<AbstractNode *> yXorChain =
            rewriteMultiInputGateToBinaryGatesChain(inputsY1ToYm, LogCompOp::LOGICAL_XOR);
        u_y_rightOperand = yXorChain.back()->castTo<AbstractExpr>();
      }
      auto *u_y = new LogicalExpr(a_t->castTo<AbstractExpr>(), LogCompOp::LOGICAL_AND, u_y_rightOperand);
      finalXorInputs.push_back(u_y);
    } // end of handling non-critical inputs y_1, ..., y_m

    // for each of these start nodes v_i
    for (auto sNode : coneStartNodes) {
      // determine critical input a_1^i and non-critical input a_2^i of v_i
      auto *sNodeAsLogicalExpr = dynamic_cast<LogicalExpr *>(sNode);
      if (sNodeAsLogicalExpr==nullptr)
        throw std::logic_error("Start node of cone is expected to be of type LogicalExpr!");
      auto[criticalInput, nonCriticalInput] = getCriticalAndNonCriticalInput(sNodeAsLogicalExpr);

      // remove critical input of v_i
      criticalInput->removeParent(sNode, false);
      sNode->removeChild(criticalInput, false);

      // remove all outgoing edges of v_i
      sNode->removeChild(criticalInput, true);

      // add non-critical input of v_t (coneEnd) named a_t as input to v_i
      auto originalOperator = sNodeAsLogicalExpr->getOperator();
      sNodeAsLogicalExpr->setAttributes(dynamic_cast<AbstractExpr *>(nonCriticalInput),
                                        originalOperator,
                                        dynamic_cast<AbstractExpr *>(a_t));

      // create new logical-AND node u_i and set v_i as input of u_i
      auto leftOp = criticalInput->castTo<AbstractExpr>();
      auto uNode = new LogicalExpr(leftOp, LogCompOp::LOGICAL_AND, sNodeAsLogicalExpr);
      finalXorInputs.push_back(uNode);
    }

    // convert multi-input XOR into binary XOR nodes
    std::vector<AbstractNode *> xorFinalGate =
        rewriteMultiInputGateToBinaryGatesChain(finalXorInputs, LogCompOp::LOGICAL_XOR);
    for (auto &gate : xorFinalGate) { gate->getUniqueNodeId(); }

    // remove coneEnd
    astToRewrite.deleteNode(&coneEnd, true);
    assert(coneEnd==nullptr);

    // connect final XOR (i.e., last LogicalExpr in the chain of XOR nodes) to cone output
    rNode->addChild(xorFinalGate.back(), false);
    xorFinalGate.back()->addParent(rNode, false);
  } //end: for (auto coneEnd : coneEndNodes)
}

std::pair<AbstractNode *, AbstractNode *> ConeRewriter::getCriticalAndNonCriticalInput(LogicalExpr *logicalExpr) {
  // check which of the two inputs is critical
  bool leftIsCritical = isCriticalNode(logicalExpr->getLeft());
  bool rightIsCritical = isCriticalNode(logicalExpr->getRight());

  // throw exceptions if both are critical or neither of both is critical
  if (leftIsCritical && rightIsCritical) {
    throw std::invalid_argument("Cannot rewrite given AST because input of cone's end node are both critical!");
  } else if (!leftIsCritical && !rightIsCritical) {
    throw std::invalid_argument("Neither input left nor input right are critical nodes!");
  }

  // return a pair of <critical input, non-critical input>
  return (leftIsCritical ? std::make_pair(logicalExpr->getLeft(), logicalExpr->getRight())
                         : std::make_pair(logicalExpr->getRight(), logicalExpr->getLeft()));
}

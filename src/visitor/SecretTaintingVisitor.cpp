#include "ast_opt/visitor/SecretTaintingVisitor.h"
#include "ast_opt/ast/FunctionParameter.h"
#include "ast_opt/ast/VarAssignm.h"
#include "ast_opt/ast/VarDecl.h"
#include "ast_opt/ast/ArithmeticExpr.h"
#include "ast_opt/ast/Return.h"
#include "ast_opt/ast/Block.h"
#include "ast_opt/ast/Variable.h"
#include "ast_opt/ast/LogicalExpr.h"
#include "ast_opt/ast/Function.h"
#include "ast_opt/ast/OperatorExpr.h"
#include "ast_opt/ast/Call.h"
#include "ast_opt/ast/If.h"
#include "ast_opt/ast/GetMatrixSize.h"
#include "ast_opt/ast/MatrixAssignm.h"
#include "ast_opt/ast/MatrixElementRef.h"

void SecretTaintingVisitor::visit(Ast &elem) {
  Visitor::visit(elem);
}

// ==========================
// STATEMENTS
// ==========================

void SecretTaintingVisitor::visit(Block &elem) {
  Visitor::visit(elem);
  // if after processing all the Block's children, any of them are tainted, this block also will be tainted
  auto statements = elem.getStatements();
  auto statementsAsNodes = std::vector<AbstractNode *>(statements.begin(), statements.end());
  if (anyNodesAreTainted(statementsAsNodes))
    addTaintedNode(elem);
}

void SecretTaintingVisitor::visit(Function &elem) {
  Visitor::visit(elem);
  // if after processing the Function's body, any of the function body's statements are tainted, this Function also will
  // be tainted (this makes sense, e.g., if Function is part of a called sub-function that is referenced by a Call obj.)
  auto bodyStatements = elem.getBodyStatements();
  auto statementsAsNodes = std::vector<AbstractNode *>(bodyStatements.begin(), bodyStatements.end());
  if (anyNodesAreTainted(statementsAsNodes)) addTaintedNode(elem);
}

void SecretTaintingVisitor::visit(If &elem) {
//  throw std::invalid_argument("ASTs containing If statements are not supported by the SecretTaintingVisitor!");
  Visitor::visit(elem);
  if (anyNodesAreTainted(elem.getChildren())) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(Return &elem) {
  Visitor::visit(elem);
  if (anyNodesAreTainted(elem.getChildren())) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(While &elem) {
  throw std::invalid_argument("ASTs containing While objects are not supported by the SecretTaintingVisitor!");
}

void SecretTaintingVisitor::visit(VarAssignm &elem) {
  // TODO: Overwriting a secret variable by a public value on all execution paths makes the variable become public.
  //  Use the ControlFlowGraphVisitor to determine the execution paths and the generated DataFlowGraph to analyze if
  //  a variable is written in all of these paths and if the written value is public.
  Visitor::visit(elem);
  // after visiting the initializer, check if it is tainted - this is needed for Call nodes
  if (nodeIsTainted(*elem.getValue())) {
    addTaintedNode(elem);
    taintedVariables.insert(elem.getIdentifier());
  }
}

void SecretTaintingVisitor::visit(VarDecl &elem) {
  if (elem.getDatatype()->isEncrypted()) {
    taintedVariables.insert(elem.getVarTargetIdentifier());
    taintedNodes.insert(elem.getUniqueNodeId());
    addTaintedNodes(elem.getDescendants());
  }
  Visitor::visit(elem);
  // after visiting the initializer, check if it is tainted - this is needed for Call nodes
  if (elem.getInitializer() != nullptr && nodeIsTainted(*elem.getInitializer())) {
    addTaintedNode(elem);
    taintedVariables.insert(elem.getIdentifier());
  }
}

// ==========================
// EXPRESSIONS
// ==========================

void SecretTaintingVisitor::visit(LogicalExpr &elem) {
  Visitor::visit(elem);
  if (anyNodesAreTainted(elem.getChildren())) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(ArithmeticExpr &elem) {
  Visitor::visit(elem);
  if (anyNodesAreTainted(elem.getChildren())) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(OperatorExpr &elem) {
  Visitor::visit(elem);
  if (anyNodesAreTainted(elem.getChildren())) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(GetMatrixSize &elem) {
  Visitor::visit(elem);
  if (anyNodesAreTainted(elem.getChildren())) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(FunctionParameter &elem) {
  // if this FunctionParameter refers to an encrypted variable -> all of its variable identifiers are tainted
  if (elem.getDatatype()->isEncrypted()) {
    // taint the FunctionParameter object
    addTaintedNode(elem);
    // remember the Variable identifiers associated
    auto varIdentifiers = elem.getVariableIdentifiers();
    addTaintedVariableIdentifiers(varIdentifiers.begin(), varIdentifiers.end());
    // taint the Variable objects
    for (auto &child : elem) {
      auto childAsVariable = dynamic_cast<const Variable*>(&child);
      if (childAsVariable!=nullptr
          && std::count(varIdentifiers.begin(), varIdentifiers.end(), childAsVariable->getIdentifier()) > 0) {
        addTaintedNode(child);
      }
    }
  }
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(ParameterList &elem) {
  Visitor::visit(elem);
  auto params = elem.getParameters();
  if (std::any_of(params.begin(),
                  params.end(),
                  [&](AbstractNode *node) { return nodeIsTainted(*node); })) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(Call &elem) {
  Visitor::visit(elem);
  // after visiting the Call's referenced Function, check if the Function node was tainted
  // if the Function was tainted -> taint the Call node too
  if (elem.getFunc()!=nullptr && nodeIsTainted(*elem.getFunc())) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(CallExternal &elem) {
  throw std::invalid_argument("ASTs containing CallExternal objects are not supported by the SecretTaintingVisitor!");
  //  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(LiteralBool &elem) {
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(LiteralInt &elem) {
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(LiteralString &elem) {
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(LiteralFloat &elem) {
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(Operator &elem) {
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(UnaryExpr &elem) {
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(Datatype &elem) {
  if (elem.isEncrypted()) addTaintedNode(elem);
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(Variable &elem) {
  if (taintedVariables.count(elem.getIdentifier()) > 0) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(For &elem) {
  //TODO: If condition depends on (reads) a secret variable throw an exception as this is extremely slow.
  // Cannot be checked yet as SecretTaintingVisitor does not support variable scopes yet.
  // Moved this check from CTES to SecretTainingVisitor, as tainting is generally not yet calculated when CTES is called
  if (false) {
    throw std::runtime_error(
        "For-loops containing secret variables are not supported because they cannot efficiently "
        "be unrolled or optimized in any other way. Aborting.");
  }
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(Rotate &elem) {
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(Transpose &elem) {
  Visitor::visit(elem);
}

void SecretTaintingVisitor::visit(MatrixElementRef &elem) {
  Visitor::visit(elem);
  if (taintedNodes.count(elem.getOperand()->getUniqueNodeId()) > 0) {
    addTaintedNode(elem);
  }
}

void SecretTaintingVisitor::visit(MatrixAssignm &elem) {
  Visitor::visit(elem);
  // after visiting the initializer, check if it is tainted - this is needed for Call nodes
  if (nodeIsTainted(*elem.getValue())) {
    addTaintedNode(elem);
    for(auto &v: elem.getAssignmTarget()->getVariableIdentifiers()) {
      taintedVariables.insert(v);
    }
  }
}

// ==========================
// HELPER METHODS
// ==========================

const std::set<std::string> &SecretTaintingVisitor::getSecretTaintingList() const {
  return taintedNodes;
}

bool SecretTaintingVisitor::nodeIsTainted(AbstractNode &node) const {
  return taintedNodes.count(node.getUniqueNodeId()) > 0;
}

bool SecretTaintingVisitor::anyNodesAreTainted(std::vector<AbstractNode *> nodes) const {
  return std::any_of(nodes.begin(), nodes.end(), [&](AbstractNode *n) {
    return taintedNodes.count(n->getUniqueNodeId()) > 0;
  });
}

void SecretTaintingVisitor::addTaintedNodes(std::vector<AbstractNode *> nodesToAdd) {
  // copy all unique node IDs of nodesToAdd to taintedNodes
  std::transform(nodesToAdd.begin(), nodesToAdd.end(), std::inserter(taintedNodes, taintedNodes.end()),
                 [](AbstractNode *n) -> std::string {
                   return n->getUniqueNodeId();
                 });
}

void SecretTaintingVisitor::addTaintedNode(const AbstractNode &n) {
  taintedNodes.insert(n.getUniqueNodeId());
}


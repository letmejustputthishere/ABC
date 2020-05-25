#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_UTILITIES_TESTUTILS_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_UTILITIES_TESTUTILS_H_

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <bitset>
#include "gtest/gtest.h"
#include "ast_opt/ast/Ast.h"
#include "ast_opt/utilities/RandNumGen.h"

/// Defines the default number of test runs to be performed, if not passed as parameter to astOutputComparer.
/// Decides whether exhaustive testing (i.e., comparing all possible inputs) will be used or not.
/// 2^12 = 4,096
static const int circuitMaxTestRuns = 4'096;

struct EvalPrinter {
 private:
  // flags to configure the amount of data to be printed
  bool flagPrintEachParameterSet{false};
  bool flagPrintVariableHeaderOnceOnly{true};
  bool flagPrintEvaluationResult{false};

  // the evaluation parameters associated to the test run
  std::unordered_map<std::string, AbstractLiteral *> *evaluationParameters;

  void ensureEvalParamsIsSet() {
    if (evaluationParameters==nullptr) {
      throw std::logic_error(
          "EvalPrinter requires calling setEvaluationParameters(...) to have access to the passed parameters.");
    }
  }

 public:
  EvalPrinter() : evaluationParameters(nullptr) {}

  EvalPrinter &setFlagPrintEachParameterSet(bool printEachParameterSet) {
    EvalPrinter::flagPrintEachParameterSet = printEachParameterSet;
    return *this;
  }

  EvalPrinter &setFlagPrintVariableHeaderOnceOnly(bool printVariableHeaderOnceOnly) {
    EvalPrinter::flagPrintVariableHeaderOnceOnly = printVariableHeaderOnceOnly;
    return *this;
  }

  EvalPrinter &setEvaluationParameters(std::unordered_map<std::string, AbstractLiteral *> *evalParams) {
    EvalPrinter::evaluationParameters = evalParams;
    return *this;
  }

  EvalPrinter &setFlagPrintEvaluationResult(bool printEvaluationResult) {
    EvalPrinter::flagPrintEvaluationResult = printEvaluationResult;
    return *this;
  }

  void printHeader() {
    if (flagPrintEachParameterSet || flagPrintEvaluationResult) {
      if (flagPrintEachParameterSet) {
        ensureEvalParamsIsSet();
        for (auto &[varIdentifier, literal] : *evaluationParameters) std::cout << varIdentifier << ", ";
        std::cout << "\b\b" << std::endl;
      }
      if (flagPrintEvaluationResult) {
        std::cout << "Evaluation Result (original / rewritten)" << std::endl;
      }
      std::cout << std::endl;
    }
  }

  void printCurrentParameterSet() {
    if (flagPrintEachParameterSet) {
      ensureEvalParamsIsSet();
      std::stringstream varIdentifiers, varValues;
      for (auto &[varIdentifier, literal] : *evaluationParameters) {
        varIdentifiers << varIdentifier << ", ";
        varValues << *literal << ", ";
      }
      varIdentifiers << "\b\b" << std::endl;
      varValues << "\b\b" << std::endl;
      std::cout << (!flagPrintVariableHeaderOnceOnly ? varIdentifiers.str() : "") << varValues.str();
    }
  }

  void printEvaluationResults(const std::vector<AbstractLiteral *> &resultExpected,
                              const std::vector<AbstractLiteral *> &resultRewrittenAst) {
    if (flagPrintEvaluationResult) {
      std::cout << "( ";
      for (auto &result : resultExpected) std::cout << result << ", ";
      std::cout << "\b\b";
      std::cout << " / ";
      for (auto &result : resultRewrittenAst) std::cout << result << ", ";
      std::cout << "\b\b";
      std::cout << " )";
    }
  }

  static void printEndOfEvaluationTestRun() {
    std::cout << std::endl;
  }
};

static void astOutputComparer(Ast &unmodifiedAst, Ast &rewrittenAst, unsigned int seed, int numTestRuns,
                              std::unordered_map<std::string, AbstractLiteral *> &evalParams,
                              EvalPrinter *evalPrinter = nullptr) {
  // create random number generator with test-specific seed
  RandLiteralGen rng(seed);

  // trigger printing of the output format (header)
  if (evalPrinter)evalPrinter->printHeader();

  for (int i = 0; i < numTestRuns; i++) {
    // generate new parameter values
    rng.randomizeValues(evalParams);

    // evaluate both ASTs with previously generated params using the appropriate evaluate function for ASTs or circuits
    std::vector<AbstractLiteral *> resultExpected, resultRewrittenAst;
    resultExpected = unmodifiedAst.evaluateAst(evalParams, false);
    resultRewrittenAst = rewrittenAst.evaluateAst(evalParams, false);


    // result plausability check
    if (resultExpected.size()!=resultRewrittenAst.size())
      throw std::runtime_error("Number of arguments in expected result and actual result does not match.");

    // trigger printing of the current parameter set and the evaluation result
    if (evalPrinter) {
      evalPrinter->printCurrentParameterSet();
      evalPrinter->printEvaluationResults(resultExpected, resultRewrittenAst);
      EvalPrinter::printEndOfEvaluationTestRun();
    }

    // compare results of original and rewritten AST
    for (int idx = 0; idx < resultExpected.size(); ++idx) {
      ASSERT_EQ(*resultExpected.at(idx), *resultRewrittenAst.at(idx));
    }
  }
}

// circuitOutputComparer with all supported parameters
static void circuitOutputComparer(Ast &unmodifiedAst,
                                  Ast &rewrittenAst,
                                  unsigned int seed,
                                  int numMaxTestRuns,
                                  std::unordered_map<std::string, AbstractLiteral *> &evalParams,
                                  EvalPrinter *evalPrinter) {
  // a function that returns True if the given evalParams entry is a LiteralBool
  auto isLiteralBool = [](const auto &mapEntry) {
    return (dynamic_cast<LiteralBool *>(mapEntry.second)!=nullptr);
  };

  // Check if we can perform exhaustive testing. We need to fall-back to randomized testing if:
  // - not all evalParams are LiteralBool (<=> there exists parameter other than type LiteralBool)
  // - or the specified maximum number of test runs would be exceeded, i.e., 2^{|evalParams|} > numMaxTestRuns
  if (!std::all_of(evalParams.begin(), evalParams.end(), isLiteralBool)
      || pow(2, evalParams.size()) > numMaxTestRuns) {
    // in either of theses cases we fall-back to the "traditional" method -> randomized testing
    return astOutputComparer(unmodifiedAst, rewrittenAst, seed, numMaxTestRuns, evalParams, evalPrinter);

  } else {
    /// This class provides a way for exhaustive testing for circuits with binary inputs.
    /// Its implementation is naive and not very efficient. However, exhaustive testing should anyway only be performed
    /// if the number of circuit inputs is small.
    struct LiteralBoolCombinationsGen {
     private:
      std::vector<LiteralBool *> params;
      std::bitset<circuitMaxTestRuns> nextBitCombination;

      void updateParams() {
        // Update all LiteralBools in params based on the current bit states in nextBitCombination.
        // Iterator 'index' is limited by params.size() because nextBitCombination is larger as its size must be known
        // at compile-time. A better approach would be to initially update all bits but in further iterations remember
        // the last modified bit and based on that exit the loop earlier.
        for (size_t index = 0; index < params.size(); ++index) {
          bool bitValue = nextBitCombination[index];
          params.at(index)->setValue(bitValue);
        }
      }

     public:
      explicit LiteralBoolCombinationsGen(std::unordered_map<std::string, AbstractLiteral *> &evalParams) {
        nextBitCombination = std::bitset<circuitMaxTestRuns>(0);
        // convert params into vector of literals
        for (auto &[varIdentifier, literal] : evalParams) params.push_back(literal->castTo<LiteralBool>());
      }

      bool hasNextAndUpdate() {
        // stop if we already built all possible combinations for 2^{params.size()} bits
        if (static_cast<double>(nextBitCombination.to_ulong()) < pow(2, params.size())) {
          // modify params map based on combination in nextBitCombination
          updateParams();
          // increment nextBitCombination map to represent next value
          nextBitCombination = nextBitCombination.to_ulong() + 1l;
          return true;
        }
        return false;
      }
    }; // struct LiteralBoolCombinationsGen

    // we can perform exhaustive testing: create a new LiteralBoolCombinationsGen instance
    LiteralBoolCombinationsGen literalBoolCombinationGen(evalParams);

    // trigger printing of the output format (header)
    if (evalPrinter)evalPrinter->printHeader();

    // as long as there are combinations of input parameters we have not tested yet -> continue
    while (literalBoolCombinationGen.hasNextAndUpdate()) {
      // perform evaluation of both ASTs using current parameter set
      auto resultExpected = unmodifiedAst.evaluateCircuit(evalParams, false);
      auto resultRewrittenAst = rewrittenAst.evaluateCircuit(evalParams, false);

      // result plausability check
      if (resultExpected.size()!=resultRewrittenAst.size())
        throw std::runtime_error("Number of arguments in expected result and actual result does not match.");

      // trigger printing of the current parameter set and the evaluation result
      if (evalPrinter) {
        evalPrinter->printCurrentParameterSet();
        evalPrinter->printEvaluationResults(resultExpected, resultRewrittenAst);
        EvalPrinter::printEndOfEvaluationTestRun();
      }

      // compare results of original and rewritten AST
      for (int idx = 0; idx < resultExpected.size(); ++idx) {
        ASSERT_EQ(*resultExpected.at(idx), *resultRewrittenAst.at(idx));
      }
    }
  }
}

// circuitOutputComparer with EvalPrinter pointer
static void circuitOutputComparer(Ast &unmodifiedAst, Ast &rewrittenAst, unsigned int seed, int numMaxTestRuns,
                                  std::unordered_map<std::string, AbstractLiteral *> &evalParams) {
  return circuitOutputComparer(unmodifiedAst, rewrittenAst, seed, numMaxTestRuns, evalParams, nullptr);
}

// circuitOutputComparer without numMaxTestRuns
static void circuitOutputComparer(Ast &unmodifiedAst, Ast &rewrittenAst, unsigned int seed,
                                  std::unordered_map<std::string, AbstractLiteral *> &evalParams) {
  return circuitOutputComparer(unmodifiedAst, rewrittenAst, seed, circuitMaxTestRuns, evalParams, nullptr);
}

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_UTILITIES_TESTUTILS_H_

#ifndef AST_OPTIMIZER_INCLUDE_AST_OPT_UTILITIES_RANDNUMGEN_H_
#define AST_OPTIMIZER_INCLUDE_AST_OPT_UTILITIES_RANDNUMGEN_H_

#include <map>
#include <queue>
#include <vector>
#include <string>
#include <random>
#include <utility>
#include <unordered_map>
#include "ast_opt/ast/AbstractStatement.h"
#include "ast_opt/ast/LiteralInt.h"
#include "ast_opt/ast/LiteralBool.h"
#include "ast_opt/ast/LiteralString.h"
#include "ast_opt/ast/LiteralFloat.h"

class RandLiteralGen {
 private:
  std::mt19937::result_type seed;
  std::mt19937 gen;
  std::uniform_int_distribution<size_t> distInt;
  std::uniform_int_distribution<size_t> distBool;
  std::uniform_int_distribution<size_t> distString;
  std::uniform_real_distribution<float> distFloat;

  static constexpr const std::pair<int, int> intRange = std::make_pair(0, 999'999);
  // note: increasing the maximum of floatRange trims the number of decimal places
  static constexpr const std::pair<double, double> floatRange = std::make_pair(0.0, 999.0);

  constexpr static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  int getRandomIntForStringGen() {
    return distString(gen);
  }

  // Credits to Carl from stackoverflow.com (https://stackoverflow.com/a/12468109/3017719)
  std::string random_string(size_t length, std::uniform_int_distribution<size_t> &distString, std::mt19937 &gen) {
    auto randchar = [this]() mutable -> char {
      const size_t maxIndex = (sizeof(charset) - 1);
      return charset[getRandomIntForStringGen()];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
  }

 public:
  static const int stringDefaultMaxLength = 12;

  explicit RandLiteralGen(std::mt19937::result_type seed)
      : seed(seed),
        distInt{intRange.first, intRange.second},
        distBool{0, 1},
        distFloat{floatRange.first, floatRange.second},
        distString{0, sizeof(charset) - 2},
        gen(seed) {}

  int getRandomInt() {
    return distInt(gen);
  }

  LiteralInt *getRandomLiteralInt() {
    return new LiteralInt(getRandomInt());
  }

  bool getRandomBool() {
    return distBool(gen);
  }

  LiteralBool *getRandomLiteralBool() {
    return new LiteralBool(getRandomBool());
  }

  std::string getRandomString(int length) {
    return random_string(length, distString, gen);
  }

  LiteralString *getRandomLiteralString(int length = stringDefaultMaxLength) {
    return new LiteralString(getRandomString(length));
  }

  float getRandomFloat() {
    return distFloat(gen);
  }

  LiteralFloat *getRandomLiteralFloat() {
    return new LiteralFloat(getRandomFloat());
  }

  void randomizeValues(const std::unordered_map<std::string, AbstractLiteral *> &vals) {
    // ask every literal to generate a new value using the passed RandNumGen instance by overwriting the existing value
    for (auto &[identifier, lit] : vals) lit->setRandomValue(*this);
  }
};

#endif //AST_OPTIMIZER_INCLUDE_AST_OPT_UTILITIES_RANDNUMGEN_H_

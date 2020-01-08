#ifndef MASTER_THESIS_CODE_ASTTESTINGGENERATOR_H
#define MASTER_THESIS_CODE_ASTTESTINGGENERATOR_H

#include <Ast.h>

/// The ASTs generated by this class are used for tests. Hence, any modifications will break the tests!
class AstTestingGenerator {
 private:
  static void genAstRewritingOne(Ast &ast);

  static void genAstRewritingTwo(Ast &ast);

  static void genAstRewritingThree(Ast &ast);

  static void genAstRewritingFour(Ast &ast);

  static void genAstRewritingFive(Ast &ast);

  static void genAstRewritingSix(Ast &ast);

  static void genAstEvalOne(Ast &ast);

 public:
  static void generateAst(int id, Ast &ast);
  static void genAstEvalTwo(Ast &ast);
  static void genAstEvalThree(Ast &ast);
  static void genAstEvalFour(Ast &ast);
  static void genAstEvalFive(Ast &ast);
  static void genAstEvalSix(Ast &ast);
  static void genAstEvalSeven(Ast &ast);
};

#endif //MASTER_THESIS_CODE_ASTTESTINGGENERATOR_H

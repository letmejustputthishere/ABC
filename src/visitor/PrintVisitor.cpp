#include "ast_opt/visitor/PrintVisitor.h"

std::string SpecialPrintVisitor::getIndentation() {
  // Indent with two spaces per level
  return std::string(2*indentation_level, ' ');
}

SpecialPrintVisitor::SpecialPrintVisitor(std::ostream &os) : os(os) {}

void SpecialPrintVisitor::visit(AbstractNode &elem) {
  // This is more of a demonstration of the Visitor template
  // since AbstractNode::toString can already output its children just fine
  // However, this PrintVisitor does technically offer the choice to indent things differently ;)

  // Get the current node's toString (without children)
  // This should hopefully be a single line, including end-of-line at the end
  std::string curNodeString = elem.toString(false);

  // Output current node at required indentation
  os << getIndentation() << curNodeString;

  // increment indentation level and visit children, decrement afterwards
  ++indentation_level;
  for(auto &c: elem) {
    visit(c);
  }
  --indentation_level;

}

#include "fields.h"

void FieldX::accept(FieldVisitor& fv) {
  fv.visit(*this);
}
void FieldO::accept(FieldVisitor& fv) {
  fv.visit(*this);
}
void FieldEmpty::accept(FieldVisitor& fv) {
  fv.visit(*this);
}
void EndGameVisitor::visit(FieldX f) {
  listener->gameEnded(f);
}
void EndGameVisitor::visit(FieldO f) {
  listener->gameEnded(f);
}
void EndGameVisitor::visit(FieldEmpty f) {
  listener->gameEnded(f);
}
void FieldTypeVisitor::visit(FieldX f) {
  result = 'x';
}
void FieldTypeVisitor::visit(FieldO f) {
  result = 'o';
}
void FieldTypeVisitor::visit(FieldEmpty f) {
  result = '0';
}

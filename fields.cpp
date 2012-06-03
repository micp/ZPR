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
bool FieldX::taken() const {
  return true;
}
Field* FieldX::clone() const {
  return new FieldX(*this);
}
bool FieldO::taken() const {
  return true;
}
Field* FieldO::clone() const {
  return new FieldO(*this);
}
bool FieldEmpty::taken() const {
  return false;
}
Field*  FieldEmpty::clone() const {
  return new FieldEmpty(*this); 
}
void EndGameVisitor::checkSwap() {
  int sX = line->getX1();
  int sY = line->getY1();
  int tX = line->getX2();
  int tY = line->getY2();
  if(sX > tX || (sX == tX && sY > tY)) line->swap();
}
void EndGameVisitor::visit(FieldX f) {
  checkSwap();
  listener->gameEnded(f, line->getX1(), line->getY1(), 
      line->getX2(), line->getY2());
}
void EndGameVisitor::visit(FieldO f) {
  checkSwap();
  listener->gameEnded(f, line->getX1(), line->getY1(), 
      line->getX2(), line->getY2());
}
void EndGameVisitor::visit(FieldEmpty f) {
  checkSwap();
  listener->gameEnded(f, line->getX1(), line->getY1(), 
      line->getX2(), line->getY2());
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

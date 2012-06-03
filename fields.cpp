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
  listener->gameEnded(f, 0 , 0 , 1 , 1);
}
void EndGameVisitor::visit(FieldO f) {
  listener->gameEnded(f, 0 , 0 , 1 , 1);
}
void EndGameVisitor::visit(FieldEmpty f) {
  listener->gameEnded(f, 0 , 0 , 1 ,1 );
}


bool FieldX::taken() const
{
  return true;
}

Field* FieldX::clone() const
{
  return new FieldX(*this);
}

char FieldX::type() const
{
  return 'x';
}


bool FieldO::taken() const
{
  return true;
}

Field* FieldO::clone() const
{
  return new FieldO(*this);
}

char FieldO::type() const
{
  return 'o';
}

bool FieldEmpty::taken() const
{
  return false;
}

Field*  FieldEmpty::clone() const
{
  return new FieldEmpty(*this); 
}

char FieldEmpty::type() const
{
  return '0';
}

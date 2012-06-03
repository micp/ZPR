#ifndef __Fieldsh__
#define __Fieldsh__

class FieldX;
class FieldO;
class FieldEmpty;
class EndOfGameListener;
#include "endofgamelistener.h"

class FieldVisitor;
class Field {
  public:
    virtual bool taken() const = 0;
    virtual void accept(FieldVisitor& fv) = 0;
    virtual Field* clone() const = 0;
  protected:
    Field() {};
  private:
    Field(const Field&) {};
}; 

class FieldX: public Field {
  public:
    FieldX() {};
    FieldX(const FieldX&) {};
    Field* clone() const;
    bool taken() const;
    void accept(FieldVisitor& fv);
};

class FieldO: public Field {
  public:
    FieldO() {};
    FieldO(const FieldO&) {};
    Field* clone() const;
    bool taken() const;
    void accept(FieldVisitor& fv);
};

class FieldEmpty: public Field {
  public:
    FieldEmpty() {};
    FieldEmpty(const FieldEmpty&) {};
    Field* clone() const;
    bool taken() const;
    void accept(FieldVisitor& fv);
};

class FieldTakenException {};

class FieldVisitor {
  public:
    virtual void visit(FieldX f) = 0;
    virtual void visit(FieldO f) = 0;
    virtual void visit(FieldEmpty f) = 0;
};
class EndGameVisitor : public FieldVisitor {
  public:
    EndGameVisitor(EndOfGameListener *l) : listener(l) {}
    void visit(FieldX f);
    void visit(FieldO f);
    void visit(FieldEmpty f);
  private:
    EndOfGameListener* listener;
};
class FieldTypeVisitor : public FieldVisitor {
  public:
    void visit(FieldX f);
    void visit(FieldO f);
    void visit(FieldEmpty f);
    char getResult() const { return result; }
  private:
    char result;
};
//#include "endofgamelistener.h"
#endif

#ifndef __Fieldsh__
#define __Fieldsh__

class FieldX;
class FieldO;
class FieldEmpty;
#include "endofgamelistener.h"

class FieldVisitor;
class Field {
  public:
    virtual bool taken() const = 0;
    virtual void accept(FieldVisitor& fv) = 0;
    virtual Field* clone() const = 0;
    //TODO tmp
    virtual char type() const = 0;
  private:
    Field(const Field&);
}; 

class FieldX: public Field {
  public:
    FieldX();
    FieldX(const FieldX&);
    bool taken() const { return true; }
    void accept(FieldVisitor& fv);
    Field* clone() const { return new FieldX(*this); }
    char type() const { return 'x'; }
};

class FieldO: public Field {
  public:
    FieldO();
    FieldO(const FieldO&);
    bool taken() const { return true; }
    void accept(FieldVisitor& fv);
    Field* clone() const { return new FieldO(*this); }
    char type() const { return 'o'; }
};

class FieldEmpty: public Field {
  public:
    FieldEmpty();
    FieldEmpty(const FieldEmpty&);
    bool taken() const { return false; }
    void accept(FieldVisitor& fv);
    Field* clone() const { return new FieldEmpty(*this); }
    char type() const { return '0'; }
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
#endif

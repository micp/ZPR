#ifndef __Fieldsh__
#define __Fieldsh__

class FieldX;
class FieldO;
class FieldEmpty;
class EndOfGameListener;
#include "endofgamelistener.h"
#include "WinningLine.h"
class FieldVisitor;
/** Abstract base class for different field types*/
class Field 
{
  public:
    /** Tests if the field is taken or not */
    virtual bool taken() const = 0;
    /** Accepts the visitor */
    virtual void accept(FieldVisitor& fv) = 0;
   /**Makes a deep copy of this field */
    virtual Field* clone() const = 0;
  protected:
    Field() {};
  private:
    Field(const Field&) {};
}; 
/**Field taken with X*/
class FieldX: public Field 
{
  public:
    FieldX() {};
    FieldX(const FieldX&) {};
    Field* clone() const;
    bool taken() const;
    void accept(FieldVisitor& fv);
};
/**Field taken with O*/
class FieldO: public Field 
{
  public:
    FieldO() {};
    FieldO(const FieldO&) {};
    Field* clone() const;
    bool taken() const;
    void accept(FieldVisitor& fv);
};
/**This field is empty*/
class FieldEmpty: public Field 
{
  public:
    FieldEmpty() {};
    FieldEmpty(const FieldEmpty&) {};
    Field* clone() const;
    bool taken() const;
    void accept(FieldVisitor& fv);
};

class FieldTakenException {};

/** This is field visitor to get some information about actual field type*/
class FieldVisitor 
{
  public:
    virtual void visit(FieldX f) = 0;
    virtual void visit(FieldO f) = 0;
    virtual void visit(FieldEmpty f) = 0;
};
/**Visitor used for calling suitable enofgamelistener method */
class EndGameVisitor : public FieldVisitor 
{
  public:
    EndGameVisitor(EndOfGameListener *l, WinningLine& ln) : listener(l),
      line(&ln) {}
    void visit(FieldX f);
    void visit(FieldO f);
    void visit(FieldEmpty f);
  private:
    void checkSwap();
    EndOfGameListener* listener;
    WinningLine* line;
};
/** Visitor used for checking actual field type */
class FieldTypeVisitor : public FieldVisitor 
{
  public:
    void visit(FieldX f);
    void visit(FieldO f);
    void visit(FieldEmpty f);
    char getResult() const { return result; }
  private:
    char result;
};
#endif

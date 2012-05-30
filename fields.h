#ifndef __Fieldsh__
#define __Fieldsh__

class Field {
  public:
    virtual bool taken() const = 0;
    //TODO tmp
    virtual char type() const = 0;
    //TODO tmp
    virtual Field* copy() = 0;
  private:
    Field(const Field&);
}; 

class FieldX: public Field {
  public:
    FieldX();
    FieldX(const FieldX&);
    bool taken() const { return true; }
    char type() const { return 'x'; }
    Field* copy() { return new FieldX(*this); }
};

class FieldO: public Field {
  public:
    FieldO();
    FieldO(const FieldO&);
    bool taken() const { return true; }
    char type() const { return 'o'; }
    Field* copy() { return new FieldO(*this); }
};

class FieldEmpty: public Field {
  public:
    FieldEmpty();
    FieldEmpty(const FieldEmpty&);
    bool taken() const { return false; }
    char type() const { return '0'; }
    Field* copy() { return new FieldEmpty(*this); }
};

class FieldTakenException {};
#endif

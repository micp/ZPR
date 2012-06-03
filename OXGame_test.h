#ifndef __OXGame_testh__
#define __OXGame_testh__

#include "endofgamelistener.h"
#include "OXGame.h"
#define BOOST_TEST_MODULE OXGame_test
#include "boost/test/included/unit_test.hpp"

class TestEndOfGameListener : public EndOfGameListener {
  public:
    TestEndOfGameListener(Game* game) : EndOfGameListener(game), callee('u') {}
    void gameEnded(FieldX f) {
      FieldTypeVisitor v;
      f.accept(v);
      callee = v.getResult();
    }
    void gameEnded(FieldO f) {
      FieldTypeVisitor v;
      f.accept(v);
      callee = v.getResult();
    }
    void gameEnded(FieldEmpty f) {
      FieldTypeVisitor v;
      f.accept(v);
      callee = v.getResult();
    }
    char calledBy() { return callee; }
  private:
    char callee;
};

char getType(const Field& f);
#endif

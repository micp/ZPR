#ifndef __OXGameh__
#define __OXGameh__

//#include "endofgamelistener.h"
class EndOfGameListener;
class Field;
class FieldX;
class FieldO;
class FieldEmpty;
class FieldVisitor;
class EndGameVisitor;
#include <set>
#include "boost/multi_array.hpp"

#ifdef __OXGAME_TEST__
#include "boost/test/minimal.hpp"
#endif

class OXGame {
  public:
    typedef boost::multi_array<Field*, 2> MapArray;
    typedef std::set<EndOfGameListener *> ListenerSet;
    OXGame(int size = 15);
    ~OXGame();
    void resetGame();
    void put(Field &f, int x, int y);
    void addEndOfGameListener(EndOfGameListener &l);
    const Field& getField(int x, int y);
    int getSize();
  private:
    class CheckIterator;
    class HorizCheckIterator;
    class VertCheckIterator;
    class SlashCheckIterator;
    class BackslashCheckIterator;

    MapArray fields_;
    ListenerSet listeners_;
    int size_;
    int winningLineLength;
    int fieldsTaken;
    bool gameEnded;

    void notifyEndOfGame(Field& f);
    void endGame();
    bool taken(const Field& f);
    bool checkLine(CheckIterator& it);
    HorizCheckIterator getHorizCheckIterator(int x, int y);
    VertCheckIterator getVertCheckIterator(int x, int y);
    SlashCheckIterator getSlashCheckIterator(int x, int y);
    BackslashCheckIterator getBackslashCheckIterator(int x, int y);
};

#include "endofgamelistener.h"
#include "fields.h"

class OXGame::CheckIterator {
  public:
    CheckIterator(const OXGame& ngame, int nx, int ny);
    CheckIterator& operator++();
    bool operator!=(CheckIterator& c);
    virtual bool hasNext() = 0;
  protected:
    int curx, cury;
    int size;
    OXGame game;
    int x, y;
    int side;
    char myType;
  private:
    virtual bool check() = 0;
    virtual bool reverseCheck() = 0;
    virtual void increment() = 0;
    virtual void reverseIncrement() = 0;
};
class OXGame::HorizCheckIterator : public CheckIterator {
  public:
    bool hasNext();
  private:
    HorizCheckIterator(const OXGame& ngame, int x, int y) :
      CheckIterator(ngame, x, y) {}
    friend OXGame::HorizCheckIterator
      OXGame::getHorizCheckIterator(int x, int y);
    bool check();
    bool reverseCheck();
    void increment() { curx++; }
    void reverseIncrement() { curx--; }
};
class OXGame::VertCheckIterator : public CheckIterator {
  public:
    bool hasNext();
  private:
    VertCheckIterator(const OXGame& ngame, int x, int y) :
      CheckIterator(ngame, x, y) {}
    friend OXGame::VertCheckIterator
      OXGame::getVertCheckIterator(int x, int y);
    bool check();
    bool reverseCheck();
    void increment() { cury++; }
    void reverseIncrement() { cury--; }
};
class OXGame::SlashCheckIterator : public CheckIterator {
  public:
    bool hasNext();
  private:
    SlashCheckIterator(const OXGame& ngame, int x, int y) :
      CheckIterator(ngame, x, y) {};
    friend OXGame::SlashCheckIterator
      OXGame::getSlashCheckIterator(int x, int y);
    bool check();
    bool reverseCheck();
    void increment() { curx++; cury++; }
    void reverseIncrement() { curx--; cury--; }
};
class OXGame::BackslashCheckIterator : public CheckIterator {
  public:
    bool hasNext();
  private:
    BackslashCheckIterator(const OXGame& ngame, int x, int y) :
      CheckIterator(ngame, x, y) {};
    friend OXGame::BackslashCheckIterator
      OXGame::getBackslashCheckIterator(int x, int y);
    bool check();
    bool reverseCheck();
    void increment() { curx--; cury++; }
    void reverseIncrement() { curx++; cury--; }
};

#endif

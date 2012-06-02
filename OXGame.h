#ifndef __OXGameh__
#define __OXGameh__

#include "endofgamelistener.h"
#include "fields.h"
#include <set>
#include "boost/multi_array.hpp"

class OXGame {
  public:
    typedef boost::multi_array<Field*, 2> MapArray;
    typedef std::set<EndOfGameListener *> ListenerSet;
    OXGame(int size = 15);
    ~OXGame();
    void resetGame();
    void put(Field &f, int x, int y);
    void addEndOfGameListener(EndOfGameListener &l);
    const Field& getField(int x, int y) const;
    int getSize() const;
  private:
    class CheckIterator;
    class HorizCheckIterator;
    class VertCheckIterator;
    class SlashCheckIterator;
    class BackslashCheckIterator;

    MapArray *fields_;  
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
    char getFieldType(int x, int y) const;
};
class OXGame::CheckIterator {
  public:
    CheckIterator(const OXGame& ngame, int nx, int ny);
    CheckIterator& operator++();
    bool operator!=(const CheckIterator& c) const;
    virtual bool hasNext() = 0;
    int getCurX() const { return curx; }
    int getCurY() const { return cury; }
  protected:
    int curx, cury;
    int size;
    const OXGame* game;
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

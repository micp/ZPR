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
#include "WinningLine.h"

/** Model class of the game.
 * This class implements the complex logic behind Noughts and Crosses.
 * The constructor takes no arguments by default, but it's possible to
 * define a different field size (the default is 15). The model doesn't
 * run any loops within, every action needs to be explicitly requested.
 */
class OXGame {
  public:
    /** The only constructor.
     * The size specifies the desired field size.
     */
    OXGame(int size = 15);
    /** OXGame destructor.
     * The only task of the destructor is to assure clean deallocation of 
     * memory to prevent memory leaks.
     */
    ~OXGame();
    /** Resets the game.
     * This functions clears the field regardless of whether the game has
     * ended or not. Use this to start the game over without creating a new
     * OXGame object
     */
    void resetGame();
    /** Puts provided Field on specified coordinates.
     * Attempts to update the game field with as requested by the arguments. 
     * If the specified field is occupied FieldTakenException is thrown. 
     * Performs automatic check whether a winning line has occurred. It also 
     * checks whether there are any remaining moves. If either of those 
     * happens, put automatically notifies all listiners registered with the 
     * addEndOfGameListener function. If the specific game has ended and 
     * hasn't been reset this function does nothing.
     */
    void put(Field &f, int x, int y);
    /** Adds a listener.
     * Adds a listener to the list of objects to be notified when the game is
     * over. The game is considered over when a valid line has been found or
     * when there are no moves left.
     */
    void addEndOfGameListener(EndOfGameListener &l);
    /** Returns Field object present at specified coordinates.
     * Returns Field object of the same type as one present on the current
     * game field at the specified coordinates.
     */
    const Field& getField(int x, int y) const;
    /** Returns current field size.
     */
    int getSize() const;
  private:
    typedef boost::multi_array<Field*, 2> MapArray;
    typedef std::set<EndOfGameListener *> ListenerSet;

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

    void notifyEndOfGame(Field& f, WinningLine& line);
    void endGame();
    bool taken(const Field& f);
    bool checkLine(CheckIterator& it, WinningLine& line);
    HorizCheckIterator getHorizCheckIterator(int x, int y);
    VertCheckIterator getVertCheckIterator(int x, int y);
    SlashCheckIterator getSlashCheckIterator(int x, int y);
    BackslashCheckIterator getBackslashCheckIterator(int x, int y);
    char getFieldType(int x, int y) const;
};

#include "endofgamelistener.h"
#include "fields.h"

class OXGame::CheckIterator {
  public:
    CheckIterator(const OXGame& ngame, int nx, int ny);
    CheckIterator& operator++();
    bool operator!=(const CheckIterator& c) const;
    virtual bool hasNext() = 0;
    int getCurX() const { return curx; }
    int getCurY() const { return cury; }
    int getX1() const { return x1; }
    int getX2() const { return x2; }
    int getY1() const { return y1; }
    int getY2() const { return y2; }
  protected:
    int curx, cury;
    int size;
    const OXGame* game;
    int x, y;
    int x1, x2, y1, y2;
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

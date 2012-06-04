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

    /** A pointer to the game field.
     * Point to two dimensional boost::multi_array containing all information
     * about the current state of the game field.
     */
    MapArray *fields_;  
    /** Set containing all registered listeners;
     * Contains pointers to all listeners registered with the game.
     */
    ListenerSet listeners_;
    /** Game field size.
     */
    int size_;
    /** Winning line length.
     * Defines how long a line needs to be to be considered a winning line.
     */
    int winningLineLength;
    /** Number of taken fields.
     * Used to determine stalemate.
     */
    int fieldsTaken;
    /** True if game has ended.
     * resetGame() switches this back to false.
     */
    bool gameEnded;

    /** Notifies all registered listeners when game ends.
     * f type determines who won (when FieldX or FieldY) or represents a 
     * stalemate (when FieldEmpty). line holds opposite ends of the winning 
     * line. If stalemate line points to (0,0) and (size_, size_).
     */
    void notifyEndOfGame(Field& f, WinningLine& line);
    /** Finishes the game.
     * Used after the game has ended.
     */
    void endGame();
    /** Determines whether a specified Field type considers itself taken.
     */
    bool taken(const Field& f);
    /** Searches for a winning line.
     * First argument determines at which angle are we searching. Second 
     * specifies where to save the winning line's coordinates, should we find
     * it.
     */
    bool checkLine(CheckIterator& it, WinningLine& line);
    /** Constructs HorizCheckIterator at specified coordinates.
     */
    HorizCheckIterator getHorizCheckIterator(int x, int y);
    /** Constructs VertCheckIterator at specified coordinates.
     */
    VertCheckIterator getVertCheckIterator(int x, int y);
    /** Constructs SlashCheckIterator at specified coordinates.
     */
    SlashCheckIterator getSlashCheckIterator(int x, int y);
    /** Constructs BackslashCheckIterator at specified coordinates.
     */
    BackslashCheckIterator getBackslashCheckIterator(int x, int y);
    /** Returns the type of field at (x, y).
     */
    char getFieldType(int x, int y) const;
};

#include "endofgamelistener.h"
#include "fields.h"

/** Winning line check iterator interface.
 * Abstract class defining constraints on line checking iterators.
 */
class OXGame::CheckIterator {
  public:
    /** CheckIterator constructor.
     * Constructs CheckIterator pointing to field at nx, ny coordinates
     * withing the provided game.
     */
    CheckIterator(const OXGame& ngame, int nx, int ny);
    /** Advance the iterator.
     * The main function of the class. Advances the iterator by one step.
     * Stops at last possible step for the specific iterator type. 
     */
    CheckIterator& operator++();
    /** Checks if equal based on curX and curY.
     */
    bool operator!=(const CheckIterator& c) const;
    /** Check if iterator can continue.
     */
    virtual bool hasNext() = 0;
    int getCurX() const { return curx; }
    int getCurY() const { return cury; }
    int getX1() const { return x1; }
    int getX2() const { return x2; }
    int getY1() const { return y1; }
    int getY2() const { return y2; }
  protected:
    /** Current iterator coordinates
     */
    int curx, cury;
    /** Size of game tied with the iterator.
     */
    int size;
    /** Game tied with the iterator.
     */
    const OXGame* game;
    /** Starting coordinates of the iterator.
     */
    int x, y;
    /** Opposite points of the current line.
     */
    int x1, x2, y1, y2;
    /** Determines current iterator direction.
     */
    int side;
    /** Determines current iterator type.
     */
    char myType;
  private:
    /** Checks if iterator can continue forward.
     */
    virtual bool check() = 0;
    /** Checks if iterator can continue backward>
     */
    virtual bool reverseCheck() = 0;
    /** Increments current iterator coordinates.
     */
    virtual void increment() = 0;
    /** Increments current iterator in reverse direction
     */
    virtual void reverseIncrement() = 0;
};
/** Horizontal check iterator.
* Checks horizontally for winning line.
*/
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
/** Vertical check iterator.
* Checks vertically for winning line.
*/
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
/** Diagonal check iterator.
* Checks diagonally (/) for winning line.
*/
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
/** Opposite diagonal check iterator.
* Checks diagonally (\) for winning line.
*/
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

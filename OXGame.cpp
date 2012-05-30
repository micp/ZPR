#include "OXGame.h"

OXGame::OXGame(int size) {
  size_ = size;
  winningLineLength = 5;
  fieldsTaken = 0;
  gameEnded = false;
  for(MapArray::iterator it = fields_.begin(); it != fields_.end(); ++it) {
    for(int i = 0; i < size_; ++i) {
      (*it)[i] = new FieldEmpty();
    }
  }
}
OXGame::~OXGame() {
  for(MapArray::iterator it = fields_.begin(); it != fields_.end(); ++it) {
    for(int i = 0; i < size_; ++i) {
      delete (*it)[i];
    }
  }
}
void OXGame::resetGame() {
  fieldsTaken = 0;
  gameEnded = false;
  for(MapArray::iterator it = fields_.begin(); it != fields_.end(); ++it) {
    for(int i = 0; i < size_; ++i) {
      delete (*it)[i];
    }
  }
  for(MapArray::iterator it = fields_.begin(); it != fields_.end(); ++it) {
    for(int i = 0; i < size_; ++i) {
      (*it)[i] = new FieldEmpty();
    }
  }
}
void OXGame::put(Field &f , int x , int y) {
  if(gameEnded) return;
  if(taken( getField(x, y) ))
    throw FieldTakenException();
  delete fields_[x][y];
  fields_[x][y] = f.clone();
  fieldsTaken++;
  if( fieldsTaken == size_ * size_) {
    FieldEmpty fe;
    notifyEndOfGame(fe);
    endGame();
  }
  HorizCheckIterator horiz = getHorizCheckIterator(x, y);
  VertCheckIterator vert = getVertCheckIterator(x, y);
  SlashCheckIterator slash = getSlashCheckIterator(x, y);
  BackslashCheckIterator backslash = getBackslashCheckIterator(x, y);
  if(checkLine(horiz) || checkLine(vert)
     || checkLine(slash) || checkLine(backslash)) {
    notifyEndOfGame(f);
    endGame();
  }
}
void OXGame::addEndOfGameListener( EndOfGameListener &l ) {
  listeners_.insert(&l);
}
const Field& OXGame::getField(int x , int y) {
  return *fields_[x][y];
}
int OXGame::getSize() {
  return size_;
}
bool OXGame::checkLine(CheckIterator& c) {
  int count;
  for(; c.hasNext(); ++c) {
    count++;
    if(count >= winningLineLength)
      return true;
  }
  return false;
}
bool OXGame::taken(const Field& f) {
  return f.taken();
}
void OXGame::notifyEndOfGame(Field& f) {
  for(ListenerSet::iterator it = listeners_.begin();
      it != listeners_.end();
      ++it) {
    EndGameVisitor visitor(*it);
    f.accept(visitor);
  }
}
OXGame::CheckIterator::CheckIterator(const OXGame& ngame, int nx, int ny) {
  game = ngame;
  size = game.getSize();
  x = curx = nx;
  y = cury = ny;
  myType = game.getField(x, y).type();
  side = 0;
}
OXGame::CheckIterator& OXGame::CheckIterator::operator++() {
  if(side == 0) {
    if(check()) increment();
    else {
      side = 1;
      curx = x;
      cury = y;
    }
  }
  else
    if(reverseCheck()) reverseIncrement();
  return *this;
}
bool OXGame::HorizCheckIterator::hasNext() {
  HorizCheckIterator h = *this;
  if(++h != *this) return true;
  return false;
}
bool OXGame::VertCheckIterator::hasNext() {
  VertCheckIterator h = *this;
  if((++h) != *this) return true;
  return false;
}
bool OXGame::SlashCheckIterator::hasNext() {
  SlashCheckIterator h = *this;
  if((++h) != *this) return true;
  return false;
}
bool OXGame::BackslashCheckIterator::hasNext() {
  BackslashCheckIterator h = *this;
  if((++h) != *this) return true;
  return false;
}
OXGame::HorizCheckIterator OXGame::getHorizCheckIterator(int x, int y) {
  return HorizCheckIterator(*this, x, y);
}
OXGame::VertCheckIterator OXGame::getVertCheckIterator(int x, int y) {
  return VertCheckIterator(*this, x, y);
}
OXGame::SlashCheckIterator OXGame::getSlashCheckIterator(int x, int y) {
  return SlashCheckIterator(*this, x, y);
}
OXGame::BackslashCheckIterator OXGame::getBackslashCheckIterator(int x, int y) {
  return BackslashCheckIterator(*this, x, y);
}
bool OXGame::CheckIterator::operator!=(OXGame::CheckIterator& c) {
  return *this != c;
}
bool OXGame::HorizCheckIterator::check() {
  return curx+1 < size && 
    game.getField(curx+1, cury).type() == myType;
}
bool OXGame::HorizCheckIterator::reverseCheck() {
  return curx-1 > 0 && 
    game.getField(curx-1, cury).type() == myType;
}
bool OXGame::VertCheckIterator::check() {
  return cury+1 < size && 
    game.getField(curx, cury+1).type() == myType;
}
bool OXGame::VertCheckIterator::reverseCheck() {
  return cury-1 > 0 && 
    game.getField(curx, cury-1).type() == myType;
}
bool OXGame::SlashCheckIterator::check() {
  return curx+1 < size && cury+1 < size && 
    game.getField(curx+1, cury+1).type() == myType;
}
bool OXGame::SlashCheckIterator::reverseCheck() {
  return curx-1 > 0 && cury-1 > 0 && 
    game.getField(curx-1, cury-1).type() == myType;
}
bool OXGame::BackslashCheckIterator::check() {
  return curx-1 > 0 && cury+1 > size && 
    game.getField(curx-1, cury+1).type() == myType;
}
bool OXGame::BackslashCheckIterator::reverseCheck() {
  return curx+1 < size && cury-1 > 0 && 
    game.getField(curx+1, cury-1).type() == myType;
}

#include "OXGame.h"

OXGame::OXGame(int size) {
  size_ = size;
  winningLineLength = 5;
  fieldsTaken = 0;
  gameEnded = false;
  fields_ = new MapArray(boost::extents[size_][size_]);
  for(MapArray::iterator it = fields_->begin(); it != fields_->end(); ++it) {
    for(int i = 0; i < size_; ++i) {
      (*it)[i] = new FieldEmpty();
    }
  }
}
OXGame::~OXGame() {
  for(MapArray::iterator it = fields_->begin(); it != fields_->end(); ++it) {
    for(int i = 0; i < size_; ++i) {
      delete (*it)[i];
    }
  }
}
void OXGame::resetGame() {
  fieldsTaken = 0;
  gameEnded = false;
  for(MapArray::iterator it = fields_->begin(); it != fields_->end(); ++it) {
    for(int i = 0; i < size_; ++i) {
      delete (*it)[i];
    }
  }
  for(MapArray::iterator it = fields_->begin(); it != fields_->end(); ++it) {
    for(int i = 0; i < size_; ++i) {
      (*it)[i] = new FieldEmpty();
    }
  }
}
void OXGame::put(Field &f , int x , int y) {
  if(gameEnded) return;
  if(taken( getField(x, y) ))
    throw FieldTakenException();
  delete (*fields_)[x][y];
  (*fields_)[x][y] = f.clone();
  fieldsTaken++;
  if(fieldsTaken >= size_ * size_) {
    FieldEmpty fe;
    WinningLine l(0, 0, size_, size_);
    notifyEndOfGame(fe, l);
    endGame();
  }
  HorizCheckIterator horiz = getHorizCheckIterator(x, y);
  VertCheckIterator vert = getVertCheckIterator(x, y);
  SlashCheckIterator slash = getSlashCheckIterator(x, y);
  BackslashCheckIterator backslash = getBackslashCheckIterator(x, y);
  WinningLine line;
  if(checkLine(horiz, line) || checkLine(vert, line)
     || checkLine(slash, line) || checkLine(backslash, line)) {
    notifyEndOfGame(f, line);
    endGame();
  }
}
void OXGame::addEndOfGameListener( EndOfGameListener &l ) {
  listeners_.insert(&l);
}
const Field& OXGame::getField(int x , int y) const {
  return *(*fields_)[x][y];
}
int OXGame::getSize() const {
  return size_;
}
void OXGame::endGame() {
  gameEnded = true;
}
bool OXGame::checkLine(CheckIterator& c, WinningLine& line) {
  int count = 1;
  for(; c.hasNext(); ++c) {
    count++;
    if(count >= winningLineLength) {
      line.setX1(c.getX1());
      line.setX2(c.getX2());
      line.setY1(c.getY1());
      line.setY2(c.getY2());
      return true;
    }
  }
  return false;
}
bool OXGame::taken(const Field& f) {
  return f.taken();
}
void OXGame::notifyEndOfGame(Field& f, WinningLine& line) {
  for(ListenerSet::iterator it = listeners_.begin();
      it != listeners_.end();
      ++it) {
    EndGameVisitor visitor(*it, line);
    f.accept(visitor);
  }
}
char OXGame::getFieldType(int x, int y) const {
  FieldTypeVisitor v;
  (*(*fields_)[x][y]).accept(v);
  return v.getResult();
}
OXGame::CheckIterator::CheckIterator(const OXGame& ngame, int nx, int ny) {
  game = &ngame;
  size = game->getSize();
  x = curx = nx;
  y = cury = ny;
  x1 = x2 = x;
  y1 = y2 = y;
  myType = game->getFieldType(nx, ny);
  side = 0;
}
OXGame::CheckIterator& OXGame::CheckIterator::operator++() {
  if(side == 0) {
    if(check()) {
      increment();
      x1 = curx;
      y1 = cury;
    }
    else {
      side = 1;
      int prevx = curx;
      int prevy = cury;
      curx = x;
      cury = y;
      if(reverseCheck()) reverseIncrement();
      else {
	curx = prevx;
	cury = prevy;
	side = 2;
      }
    }
  }
  else if(side == 1) {
    if(reverseCheck()) {
      reverseIncrement();
      x2 = curx;
      y2 = cury;
    }
    else {
      //x2 = curx;
      //y2 = cury;
      side = 2;
    }
  }
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
bool OXGame::CheckIterator::operator!=(const OXGame::CheckIterator& c) const {
  return curx != c.getCurX() || cury != c.getCurY();
}
bool OXGame::HorizCheckIterator::check() {
  return curx+1 < size && 
    game->getFieldType(curx+1, cury) == myType;
}
bool OXGame::HorizCheckIterator::reverseCheck() {
  return curx-1 >= 0 && 
    game->getFieldType(curx-1, cury) == myType;
}
bool OXGame::VertCheckIterator::check() {
  return cury+1 < size && 
    game->getFieldType(curx, cury+1) == myType;
}
bool OXGame::VertCheckIterator::reverseCheck() {
  return cury-1 >= 0 && 
    game->getFieldType(curx, cury-1) == myType;
}
bool OXGame::SlashCheckIterator::check() {
  return curx+1 < size && cury+1 < size && 
    game->getFieldType(curx+1, cury+1) == myType;
}
bool OXGame::SlashCheckIterator::reverseCheck() {
  return curx-1 >= 0 && cury-1 >= 0 && 
    game->getFieldType(curx-1, cury-1) == myType;
}
bool OXGame::BackslashCheckIterator::check() {
  return curx-1 >= 0 && cury+1 < size && 
    game->getFieldType(curx-1, cury+1) == myType;
}
bool OXGame::BackslashCheckIterator::reverseCheck() {
  return curx+1 < size && cury-1 >= 0 && 
    game->getFieldType(curx+1, cury-1) == myType;
}

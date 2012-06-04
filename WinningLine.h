#ifndef __WINNING_LINEH__
#define __WINNING_LINEH__
/** Used for finding winning line*/
class WinningLine {
  public:
    WinningLine() : x1(0), x2(0), y1(0), y2(0) {}
    WinningLine(int nx1, int ny1, int nx2, int ny2) : 
      x1(nx1), y1(ny1), x2(nx2), y2(ny2) {}
    void setX1(int n) { x1 = n; }
    void setX2(int n) { x2 = n; }
    void setY1(int n) { y1 = n; }
    void setY2(int n) { y2 = n; }
    int getX1() const { return x1; }
    int getX2() const { return x2; }
    int getY1() const { return y1; }
    int getY2() const { return y2; }
    void swap();
  private:
    int x1, x2, y1, y2;
};
#endif

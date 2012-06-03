#include "WinningLine.h"

void WinningLine::swap() {
  x1 += x2;
  x2 = x1 - x2;
  x1 -= x2;
  y1 += y2;
  y2 = y1 - y2;
  y1 -= y2;
}

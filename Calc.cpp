#include "Calc.h"

Calc::Calc(){

}

// radial collision detection
bool Calc::collision(int x1, int y1, int x2, int y2, int radius){
  long a = abs(x1 - x2);
  long b = abs(y1 - y2);
  long c = round(sqrt(pow(a, 2) + pow(b, 2)));
  if(c <= radius){
    return true;
  }
  return false;
}

// distance between two 2D positions
int Calc::distance(int _x1, int _y1, int _x2, int _y2){
  long a = abs(_x1 - _x2);
  long b = abs(_y1 - _y2);
  long c = round(sqrt(pow(a, 2) + pow(b, 2)));
  return c;
}
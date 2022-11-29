#ifndef Calc_h
#define Calc_h

#include "Arduino.h"

class Calc{
  public:
    Calc();
    // radial collision detection
    bool collision(int x1, int y1, int x2, int y2, int radius);
    
    // distance between two 2D positions
    int distance(int _x1, int _y1, int _x2, int _y2);
};

#endif
#ifndef Input_h
#define Input_h

#include <Adafruit_FT6206.h>

class Input{
  protected:
    Adafruit_FT6206 ts;

  public:
    Input();

    int x;
    int y;

    int previous_x;
    int previous_y;

    // Active = There is an input/ touch at the current cycle.
    bool active;
    
    // Initialize the Touchscreen & Get Touchposition
    void setup();
    void update();
};

#endif
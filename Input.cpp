#include "Input.h"

Input::Input(){
  ts = Adafruit_FT6206();
  x = 0;
  y = 0;
  active = false;
}

// Initialize the Touchscreen.
void Input::setup(){
  if(! ts.begin(40)){
    while(1);
  } else {
  }
}

// Get touchposition, if there is any.
void Input::update(){
  if(!ts.touched()){
    active = false;
    return;
  }

  active = true;

  x = ts.getPoint().x;
  y = ts.getPoint().y;

  x = map(x, 0, 240, 240, 0);
  y = map(y, 0, 320, 320, 0);
}
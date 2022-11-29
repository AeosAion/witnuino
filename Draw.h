#ifndef Draw_h
#define Draw_h

// TFT
#include <stdint.h>
#include "Adafruit_ILI9341.h"
#define TFT_CS (uint8_t)10
#define TFT_DC (uint8_t)9

#include "Globals.h"
#include "Grid.h"

class Draw{
  protected:
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    Grid* grid;

  public:
    Draw();
    Draw(Grid* _grid);

    void setup();
    
    // backlighting
    void fade(uint8_t delay_time, int from_brightness, int to_brightness);
    
    // puzzle drawing
    void puzzle();
    void element(uint8_t _gx, uint8_t _gy);
    void colour_element(uint8_t _gx, uint8_t _gy, int _colour);

    // primitives
    void fill_screen(int _colour);
    void fill_circle(int _x, int _y, int _r, int _colour);
    void line(int _x0, int _y0, int _x1, int _y1, int _width, int _colour);  // VERTICAL OR HORIZONTAL ONLY!!

    // primitives on grid coordinates
    void gline(uint8_t _gx0, uint8_t _gy0, uint8_t _gx1, uint8_t _gy1, int _width, int _colour); // VERTICAL OR HORIZONTAL ONLY!!
    void gfill_circle(uint8_t _gx, uint8_t _gy, int _r, int _colour);

  private:  
    void backlighting(int brightness);  

    // walkable grid
    void edges();
    void endcap(uint8_t _gx, uint8_t _gy, int _colour);

    // puzzle elements
    void elements();
    void triangle(uint8_t _gx, uint8_t _gy, uint8_t _amnt, int _colour);
    void hex(uint8_t _gx, uint8_t _gy, int _colour);
};

#endif
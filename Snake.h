#ifndef Snake_h
#define Snake_h

#include "Globals.h"
#include "Grid.h"
#include "Draw.h"
#include "Input.h"

class Snake{
  protected:  
    Grid* grid;
    Draw* draw;
    Input* input;

  public:
    Snake();
    Snake(Grid* _grid, Input* _input, Draw* _draw);
    
    // pixel position
    int x = 0;
    int y = 0;

    // grid position
    uint8_t gx = 0;
    uint8_t gy = 0;

    // current path length / index.
    uint8_t path_index = 0;   

    // last pathpos on endcap and no input
    bool onEnd = false;

    void setup();
    void update(); 

    // return elements from snake path
    uint8_t get_gx_at(uint8_t _index);
    uint8_t get_gy_at(uint8_t _index);
    uint8_t get_last_gx();
    uint8_t get_last_gy();

    // returns whether snake path contains a grid coordinate
    bool contains(uint8_t _gx, uint8_t _gy);

    // draws snake path
    void draw_path(int _colour);

  private:
    // tells whether the snake has been activated or not. i know...
    bool active = false;

    // changes the snake position
    void move(uint8_t _dir); 

    // adds or removes pathpoints
    void path_handling();

    // determines direction of snake to touch
    uint8_t direction(uint8_t gcx, uint8_t gcy);

    // sets startpoint
    void set_start(uint8_t _gx, uint8_t _gy);

    // the almighty path
    uint8_t path[27][2];
    uint8_t contains_at(uint8_t _gx, uint8_t _gy);
};

#endif
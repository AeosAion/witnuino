#ifndef Grid_h
#define Grid_h

#include <stdint.h>
#include "Globals.h"
#include "DataHandler.h"

class Grid{
  public:
    Grid();
    Grid(uint8_t _level_index);

    // level index
    uint8_t level_index = 0;            
    
    // grid position
    uint8_t x = 0;              
    uint8_t y = 0;              

    // grid size & spacing
    uint8_t sx = 0;
    uint8_t sy = 0;
    uint8_t spacing = 0;

    // puzzle colours
    int bg_colour;
    int grid_colour;
    int path_colour;

    uint8_t level_digits[3];

    bool setup();

    // translate grid coordinate to pixel coordinate
    int gpx(uint8_t _gx);
    int gpy(uint8_t _gy);

    // return type at grid coordinate 
    uint8_t get_type(uint8_t _gx, uint8_t _gy);

    // return walkable at grid coordinate
    bool walkable(uint8_t _gx, uint8_t _gy);

    // bool write_save(uint8_t _level_index);

  private:   
    // array containing types at gridcoordinates [gx][gy]
    uint8_t elements_map[9][9]; 
    void load_map();
    
    // calculating position and spacing of grid for centering.
    void calc_pos();
};

#endif
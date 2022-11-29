#include <stdint.h>
#ifndef Solver_h
#define Solver_h

#include "Globals.h"
#include "Snake.h"
#include "Draw.h"

class Solver {
  protected:
    Snake* snake;
    Grid* grid;
    Draw* draw;

  public:
    Solver();
    Solver(Snake* _snake, Grid* _grid, Draw* _draw);

    // Goes through all validate functions.
    bool validate();

    // validates the hex elements
    bool validate_hexs();
    bool validate_hex(uint8_t _gx, uint8_t _gy);
    
    // validates the triangle elements
    bool validate_triangles();
    bool validate_triangle(uint8_t _gx, uint8_t _gy);
    
    // validates all region elements. for now only squares
    // basically a floodfill algorithm with region markers.
    // TODO: sun elements.
    void validate_regions(uint8_t _gx, uint8_t _gy);

    // flashes all invalid elements.
    void flash_invalids();

  private:
    // counter for invalid elements & array with gridpositions
    uint8_t num_invalid = 0;
    uint8_t invalids[16][2];
    void invalids_add(uint8_t _gx, uint8_t _gy);
};

#endif
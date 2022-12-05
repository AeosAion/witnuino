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

    // HEXs
    bool validate_hexs();
    bool validate_hex(uint8_t _gx, uint8_t _gy);
    
    // TRIANGLES
    bool validate_triangles();
    bool validate_triangle(uint8_t _gx, uint8_t _gy);

    // SQUARES
    bool validate_squares();
    
    // FLOODFILL
    // Basically a floodfill algorithm filling the region_map with region flags.
    void floodfill_regions(uint8_t _gx, uint8_t _gy);

    // FLASHES all invalid elements.
    void flash_invalids();

  private:
    // REGION map + region counter
    // The region map can be half the grid size, to save memory, because we do not need to look at the edges.
    uint8_t num_regions = 1;
    uint8_t region_map[4][4];

    // INVALIDS + counter for invalid elements & array with gridpositions
    uint8_t num_invalid = 0;
    uint8_t invalids[16][2];
    void invalids_add(uint8_t _gx, uint8_t _gy);
};

#endif
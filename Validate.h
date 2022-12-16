#include <stdint.h>
#ifndef Validate_h
#define Validate_h

#include "Globals.h"
#include "Snake.h"
#include "Draw.h"

class Validate {
  protected:
    Snake* snake;
    Grid* grid;
    Draw* draw;

  public:
    Validate();
    Validate(Snake* _snake, Grid* _grid, Draw* _draw);

    // Goes through all validate functions.
    bool run();

    bool validate_hts();

    // HEXs
    bool validate_hexs();
    bool validate_hex(uint8_t _gx, uint8_t _gy);
    
    // TRIANGLES
    bool validate_triangles();
    bool validate_triangle(uint8_t _gx, uint8_t _gy);

    // SQUARES
    bool validate_square(uint8_t gx, uint8_t gy);
    // bool validate_squares();
    
    // SUNS
    bool validate_suns();
    
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

    // returns the amount of defined colour in the defined region 
    uint8_t colour_in_region(uint8_t colour_index, uint8_t region_index, uint8_t colour_map[4][4]);

    // INVALIDS + counter for invalid elements & array with gridpositions
    uint8_t num_invalid = 0;
    uint8_t invalids[16][2];
    void invalids_add(uint8_t _gx, uint8_t _gy);
};

#endif
#include <stdint.h>
#include "Validate.h"

Validate::Validate() {
}

Validate::Validate(Snake* _snake, Grid* _grid, Draw* _draw) {
  snake = _snake;
  grid = _grid;
  draw = _draw;
}

// Main function that goes through all puzzle element validation functions.
// TODO: Optimize. When theres no region puzzle elements dont validate_regions.
bool Validate::run() {
  bool valid = true;

  floodfill_regions(1, 1);

  if (!validate_hts()){
    valid = false;
  }

  // if (!validate_squares()){
  //   valid = false;
  // }

  if (!validate_suns()){
    valid = false;
  }

  if (num_invalid != 0) {
    valid = false;
  }

  if (valid) {
    // write to savefile
  }
  return valid;
}

bool Validate::validate_hts(){
  bool valid = true;

  for (int i = 0; i < grid->sy; i++) {
    for (int j = 0; j < grid->sx; j++) {
      uint8_t type = grid->get_type(j, i);

      if (type == P_HEX) {
        if (!validate_hex(j, i)) {
          valid = false;
        }
      }

      if (type >= C_TRI1 && type <= C_TRI3) {
        if (!validate_triangle(j, i)) {
          valid = false;
        }
      }
   
      if  (type >= C_SQ_B && type <= C_SQ_ORANGE) {
        if(!validate_square(j, i)){
          valid = false;
        }
      }     
    }
  }
  return valid;
}

bool Validate::validate_triangle(uint8_t _gx, uint8_t _gy) {
  uint8_t type = grid->get_type(_gx, _gy);
  uint8_t desire = type - (C_TRI1 - 1);
  uint8_t amnt = 0;

  for (int i = 0; i < snake->path_index; i++) {
    if (abs(snake->get_gx_at(i) - _gx) == 1 && abs(snake->get_gy_at(i) - _gy) == 1) {
      if (abs(snake->get_gx_at(i + 1) - _gx) == 1 && abs(snake->get_gy_at(i + 1) - _gy) == 1) {
        amnt++;
      }
    }
  }

  if (amnt != desire) {
    invalids_add(_gx, _gy);
    return false;
  }
  return true;
}

bool Validate::validate_hex(uint8_t _gx, uint8_t _gy) {
  for (int i = 0; i <= snake->path_index; i++) {
    if (snake->get_gx_at(i) == _gx && snake->get_gy_at(i) == _gy) {
      return true;
    }
  }
  invalids_add(_gx, _gy);
  return false;
}

void Validate::flash_invalids() {
  for (int k = 0; k < 5; k++) {
    for (int i = 0; i < num_invalid; i++) {
      draw->colour_element(invalids[i][0], invalids[i][1], RED);
    }
    delay(250);

    for (int i = 0; i < num_invalid; i++) {
      draw->element(invalids[i][0], invalids[i][1]);
    }
    delay(250);
  }
}

void Validate::invalids_add(uint8_t _gx, uint8_t _gy) {
  invalids[num_invalid][0] = _gx;
  invalids[num_invalid][1] = _gy;
  num_invalid++;
}

void Validate::floodfill_regions(uint8_t _gx, uint8_t _gy) {
  uint8_t visited[9][9];
  uint8_t visit[81][2];
  uint8_t region = 1;

  // first set all to unvisited
  for (int i = 0; i < grid->sy; i++) {
    for (int j = 0; j < grid->sx; j++) {
      visited[j][i] = 0;
    }
  }

  // main floodfill loop
  // basically an array treated in a stack manner
  for (int i = 1; i < grid->sy - 1; i++) {
    for (int j = 1; j < grid->sx - 1; j++) {
      
      // VISITED, skip to next.
      if (visited[j][i] != 0) {
        goto nextregion;
      }

      // UNVISITED add to VISIT and set as start index
      if (visited[j][i] == 0) {
        visit[0][0] = j;
        visit[0][1] = i;

        // then we set the start index (in the array)
        int k = 0;

        // now we take "stack-array" from (start index) top to bottom, where j indicates top pos
        while (k > -1) {
          uint8_t n = k;

          uint8_t current_x = visit[n][0];
          uint8_t current_y = visit[n][1];

          // check if already visited and skip if so
          if (visited[current_x][current_y] != 0) {
            goto next;
          }

          // set current pos to visited with region flag
          visited[current_x][current_y] = region;

          // PUZZLE BORDER CHECK
          if (current_x < 1 || current_x >= grid->sx - 1 || current_y < 1 || current_y >= grid->sy - 1) {
            // draw->fill_circle(grid->gpx(current_x), grid->gpy(current_y), 2, RED); // visual debugging
            goto next;
          }

          // PATH CHECK
          if (snake->contains(current_x, current_y)) {
            // draw->fill_circle(grid->gpx(current_x), grid->gpy(current_y), 2, RED); // visual debugging
            goto next;
          }

          // VALID
          // To save memory we are only storing cells, and dismiss the edges.
          // Thus we take the current x and y and divide it by two. 
          // It will be rounded to the digit before the decimel.
          region_map[current_x / 2][current_y / 2] = region;
          
          // visual debugging
          // draw->fill_circle(20 * region, 20, 4, region * 50);
          // draw->fill_circle(grid->gpx(current_x), grid->gpy(current_y), 2, region * 50);

          // add BOTTOM cell to array
          visit[k][0] = current_x;
          visit[k][1] = current_y + 1;
          k++;
          // add LEFT cell to array
          visit[k][0] = current_x - 1;
          visit[k][1] = current_y;
          k++;
          // add RIGHT cell to array
          visit[k][0] = current_x + 1;
          visit[k][1] = current_y;
          k++;
          // add TOP cell to array
          visit[k][0] = current_x;
          visit[k][1] = current_y - 1;
          k++;

          next:
          k--;
          // delay(50); // delay for visual debugging purposes
        }
        num_regions++;
        region++;
      }
      nextregion:
      continue;
    }
  }
}

bool Validate::validate_square(uint8_t gx, uint8_t gy){
  uint8_t type = grid->get_type(gx, gy);
  uint8_t region = region_map[gx / 2][gy / 2];

  for (int i = 0; i < (grid->sy / 2); i++) {
    for (int j = 0; j < (grid->sx / 2); j++) {

      if(region_map[j][i] != region){
        continue;
      }
    
      uint8_t map_x = (j * 2) + 1;
      uint8_t map_y = (i * 2) + 1;

      uint8_t curr_type = grid->get_type(map_x, map_y);

      if(curr_type < C_SQ_B || curr_type > C_SQ_ORANGE) {
        continue;
      }
          
      if(grid->get_type(map_x, map_y) != type && grid->get_type(map_x, map_y)){
        invalids_add(gx, gy);
        return false;
      }
    }
  }

  return true;
}


// The following is a bit unfortunate:
// To save memory (RAM) I chose to not create a Cell class that 
// would hold the position, type of cell, colour etc.. I think that would
// be the nicest way to handle this, though it'd eat through so much more memory. 
// Currently the grid is a [9][9] array of uint8_t which is slim 81 bytes. 
// Having a Cell class with just the type and colour would double the size.

bool Validate::validate_suns(){
  bool valid = true;
  uint8_t colour_map[4][4]; // 0 = NONE, 1 = BLACK, 2 = WHITE, 3 = BLUE, 4 = GREEN, 5 = ORANGE

  // clear and init the colour map
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      colour_map[j][i] = 255;
    }
  }

  // first fill the colour map
  for(int i = 0; i < (grid->sy / 2); i++){
    for(int j = 0; j < (grid->sx / 2); j++){
      uint8_t map_x = (j * 2) + 1;
      uint8_t map_y = (i * 2) + 1;
      uint8_t type = grid->get_type(map_x, map_y);     

      if(type >= C_SQ_B && type <= C_SQ_ORANGE){
        uint8_t colour_index = type - C_SQ_B;
        colour_map[j][i] = colour_index;
      }

      if(type >= C_SUN_B && type <= C_SUN_ORANGE){
        uint8_t colour_index = type - C_SUN_B;
        colour_map[j][i] = colour_index;
      }
      
      if(type >= C_TRI1 && type <= C_TRI3){
        colour_map[j][i] = 4;
      }
    }
  }

  // then checking each sun for its region and amount of same coloured in that region
  // if the amount is over 1 its invalid.
  for(int i = 0; i < (grid->sy / 2); i++){
    for(int j = 0; j < (grid->sx / 2); j++){
      uint8_t map_x = (j * 2) + 1;
      uint8_t map_y = (i * 2) + 1;
      uint8_t type = grid->get_type(map_x, map_y);      
      
      if (type >= C_SUN_B && type <= C_SUN_ORANGE){     
        if(colour_in_region(colour_map[j][i], region_map[j][i], colour_map) != 2){
          invalids_add(map_x, map_y);
          valid = false;
        }
      }
    }
  }

  return valid;
}

uint8_t Validate::colour_in_region(uint8_t colour_index, uint8_t region_index, uint8_t colour_map[4][4]){
  uint8_t colour_counter = 0;

  for(int i = 0; i < (grid->sy / 2); i++){
    for(int j = 0; j < (grid->sx / 2); j++){
      if(region_map[j][i] == region_index && colour_map[j][i] == colour_index){
        colour_counter += 1;
      }      
    }
  }
  return colour_counter;
}


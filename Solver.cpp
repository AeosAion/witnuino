#include <stdint.h>
#include "Solver.h"

Solver::Solver() {
}

Solver::Solver(Snake* _snake, Grid* _grid, Draw* _draw) {
  snake = _snake;
  grid = _grid;
  draw = _draw;
}

// Main function that goes through all puzzle element validation functions.
// TODO: Optimize. When theres no region puzzle elements dont validate_regions.
bool Solver::validate() {
  bool valid = true;

  floodfill_regions(1, 1);

  if (!validate_hexs()) {
    valid = false;
  }

  if (!validate_triangles()) {
    valid = false;
  }

  if (!validate_squares()){
    valid = false;
  }

  if (num_invalid != 0) {
    valid = false;
  }

  if (valid) {
    DataHandler dh = DataHandler();
    dh.setup();
    dh.open_txt_file("save");

    uint8_t level_digits[3];
    for(int i = 0; i < 3; i++){
      level_digits[i] = grid->level_digits[i];
    }
    
    level_digits[2] += 1;

    if(level_digits[2] > 9){
      level_digits[2] = 0;
      level_digits[1] += 1;

      if((level_digits[1] + 1) > 9){
        level_digits[1] = 0;
        level_digits[0] += 1;
      }
    }

    for(int i = 0; i < 3; i++){
      dh.write_next(level_digits[i] + '0');
    }
    dh.close_file();
  }

  return valid;
}

//
bool Solver::validate_triangles() {
  bool valid = true;

  for (int i = 0; i < grid->sy; i++) {
    for (int j = 0; j < grid->sx; j++) {
      uint8_t type = grid->get_type(j, i);

      if (type >= C_TRI1 && type <= C_TRI3) {
        if (!validate_triangle(j, i)) {
          valid = false;
        }
      }
    }
  }
  return valid;
}

bool Solver::validate_triangle(uint8_t _gx, uint8_t _gy) {
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

bool Solver::validate_hex(uint8_t _gx, uint8_t _gy) {
  for (int i = 0; i <= snake->path_index; i++) {
    if (snake->get_gx_at(i) == _gx && snake->get_gy_at(i) == _gy) {
      return true;
    }
  }
  invalids_add(_gx, _gy);
  return false;
}

bool Solver::validate_hexs() {
  bool valid = true;

  for (int i = 0; i < grid->sy; i++) {
    for (int j = 0; j < grid->sx; j++) {
      uint8_t type = grid->get_type(j, i);

      if (type == P_HEX) {
        if (!validate_hex(j, i)) {
          valid = false;
        }
      }
    }
  }
  return valid;
}

void Solver::flash_invalids() {
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

void Solver::invalids_add(uint8_t _gx, uint8_t _gy) {
  invalids[num_invalid][0] = _gx;
  invalids[num_invalid][1] = _gy;
  num_invalid++;
}

void Solver::floodfill_regions(uint8_t _gx, uint8_t _gy) {
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
          // delay(20); // delay for visual debugging purposes
        }
        num_regions++;
        region++;
      }
      nextregion:
      continue;
    }
  }
}

bool Solver::validate_squares(){
  for(int region = 1; region <= num_regions; region++){
    uint8_t region_square = 0;
    bool flash_region_squares = false;

    // going through all cells
    for(int i = 0; i < (grid->sy / 2); i++){
      for(int j = 0; j < (grid->sx / 2); j++){
        uint8_t map_x = (j * 2) + 1;
        uint8_t map_y = (i * 2) + 1;
        
        // only looking at elements in the current region
        if(region_map[j][i] == region){  
          uint8_t type = grid->get_type(map_x, map_y);

          // if its a square we cant have diffenrtly coloured in a region
          if (type == C_SQ_B || type == C_SQ_W) {
            if (region_square == 0) {
              region_square = type;
            }
            if (type != region_square) {
              flash_region_squares = true;
            }
          }

        }
      }
    }

    if(flash_region_squares){
      for(uint8_t i = 0; i < (grid->sy / 2); i++){
        for(uint8_t j = 0; j < (grid->sy / 2); j++){
          if (region_map[j][i] == region){
            uint8_t map_x = (j * 2) + 1;
            uint8_t map_y = (i * 2) + 1;
            uint8_t type = grid->get_type(map_x, map_y);
            if (type == C_SQ_B || type == C_SQ_W){
              invalids_add(map_x, map_y);
            }
          }
        }
      }
    }
  }
}

// bool Solver::validate_suns(){
//   for(int i = 0; i < (grid->sy / 2); i++){
//     for(int j = 0; j < (grid->sx / 2); j++){
      
//     }
//   }
// }
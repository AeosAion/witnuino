#include <stdint.h>
#include "Solver.h"

Solver::Solver(){

}

Solver::Solver(Snake* _snake, Grid* _grid, Draw* _draw){
  snake = _snake;
  grid = _grid;
  draw = _draw;
}

// Main function that goes through all puzzle element validation functions.
// TODO: Optimize. When theres no region puzzle elements dont validate_regions.
bool Solver::validate(){
  bool valid = true; 
  
  if(!validate_hexs()){
    valid = false;
  }

  if(!validate_triangles()){
    valid = false;
  }

  validate_regions(1, 1);

  if(num_invalid != 0){
    valid = false;
  }

  return valid;
}

// 
bool Solver::validate_triangles(){
  bool valid = true;

  for(int i = 0; i < grid->sy; i++){
    for(int j = 0; j < grid->sx; j++){
      uint8_t type = grid->get_type(j, i);

      if(type >= C_TRI1 && type <= C_TRI3){
        if(!validate_triangle(j, i)){
          valid = false;
        }
      }    
    }
  }
  return valid;
}

bool Solver::validate_triangle(uint8_t _gx, uint8_t _gy){
  uint8_t type = grid->get_type(_gx, _gy);
  uint8_t desire = type - (C_TRI1 - 1);
  uint8_t amnt = 0;

  for(int i = 0; i < snake->path_index; i++){
    if(abs(snake->get_gx_at(i) - _gx) == 1 && abs(snake->get_gy_at(i) - _gy) == 1){
      if(abs(snake->get_gx_at(i + 1) - _gx) == 1 && abs(snake->get_gy_at(i + 1) - _gy) == 1){
        amnt++;
      }     
    }
  }

  if(amnt != desire){
    invalids_add(_gx, _gy);
    return false;
  }
  return true;
}

bool Solver::validate_hex(uint8_t _gx, uint8_t _gy){
  for(int i = 0; i <= snake->path_index; i++){
    if(snake->get_gx_at(i) == _gx && snake->get_gy_at(i) == _gy){
      return true;
    }
  }
  invalids_add(_gx, _gy);
  return false;
}

bool Solver::validate_hexs(){
  bool valid = true;

  for(int i = 0; i < grid->sy; i++){
    for(int j = 0; j < grid->sx; j++){
      uint8_t type = grid->get_type(j, i);

      if(type == P_HEX){
        if(!validate_hex(j, i)){
          valid = false;
        }
      }    
    }
  }
  return valid;
}

void Solver::flash_invalids(){
  for(int k = 0; k < 5; k++){
      for(int i = 0; i < num_invalid; i++){
        draw->colour_element(invalids[i][0], invalids[i][1], RED);
    }
    delay(250);

    for(int i = 0; i < num_invalid; i++){
      draw->element(invalids[i][0], invalids[i][1]);
    }
    delay(250);
  }
}

void Solver::invalids_add(uint8_t _gx, uint8_t _gy){
  invalids[num_invalid][0] = _gx;
  invalids[num_invalid][1] = _gy;
  num_invalid++;
}

void Solver::validate_regions(uint8_t _gx, uint8_t _gy){
  uint8_t visited[9][9];
  uint8_t visit[81][2];
  uint8_t region_squares[9];
  uint8_t region = 1;

  for(int i = 0; i < 9; i++){
    region_squares[i] = 0;
  } 

  // first set all to unvisited
  for(int i = 0; i < grid->sy; i++){
    for(int j = 0; j < grid->sx; j++){
      visited[j][i] = 0;
    }
  }
  
  // set regionmarkers
  for(int i = 1; i < grid->sy - 1; i++){
    for(int j = 1; j < grid->sx - 1; j++){
      
        if(visited[j][i] != 0){
          goto nextregion;
        }

        if(visited[j][i] == 0){
          // then set the starting point to a visit location
          visit[0][0] = j;
          visit[0][1] = i;
          int k = 0;

          // take "stack-array" from top to bottom, where j indicates top pos
          while(k > -1){
            uint8_t n = k;

            uint8_t current_x = visit[n][0];
            uint8_t current_y = visit[n][1];

            // check if already visited and skip if so
            if(visited[current_x][current_y] != 0){
              goto next;
            }   
                     
            // set current pos to visited
            visited[current_x][current_y] = region;

            // check for borders and skip if so
            if(current_x < 1 || current_x >= grid->sx - 1 || current_y < 1 || current_y >= grid->sy - 1){
              // visual debugging
              // draw->fillCircle(grid->gpx(current_x), grid->gpy(current_y), 2, RED); 
              goto next;
            }    

            // check if path is there and skip if so
            if(snake->contains(current_x, current_y)){
              // visual debugging
              // draw->fillCircle(grid->gpx(current_x), grid->gpy(current_y), 2, RED); 
              goto next;
            }   

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
            
            // visual debugging
            // draw->fillCircle(20 * region, 20, 4, region * 50); 
            // draw->fillCircle(grid->gpx(current_x), grid->gpy(current_y), 2, region * 50); 
            
            uint8_t type = grid->get_type(current_x, current_y);
            
            if(type == C_SQ_B || type == C_SQ_W){
              if(region_squares[region] == 0){
                region_squares[region] = type;
              } 
              if(type != region_squares[region]){
                invalids_add(current_x, current_y);
              } 
            }

            next:
            k--;
            // delay(100); // delay for visual debugging purposes
          }
          region++;
        }
        nextregion:
        continue;
        // delay(10);
    }
  }
  
  

  // TODO: check rounds and suns for region
  // for(uint8_t i = 1; i < grid->sy; i = i + 2){
  //   for(uint8_t j = 1; j < grid->sy; j = j + 2){
  //     uint8_t c_region = visited(j, i);
  //     if(type == ) 
  //   }
  // }
  
}
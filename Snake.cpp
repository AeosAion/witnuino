#include "Snake.h"

Snake::Snake(){
}


Snake::Snake(Grid* _grid, Input* _input, Draw* _draw){
  grid = _grid;
  input = _input;
  draw = _draw;
}


void Snake::setup(){
  active = false;
}

// sets the startpos of the path
void Snake::set_start(uint8_t _gx, uint8_t _gy){
  gx = _gx;
  gy = _gy;
  
  x = grid->gpx(gx); 
  y = grid->gpy(gy);

  path_index = 0;
  path[path_index][0] = gx;  
  path[path_index][1] = gy;

  onEnd = false;
}

// loop handling of the snake
void Snake::update(){
  // if no input, no need to calculate anything... 
  // except for if it was active before, we might have stopped at the endpoint!
  // if so we draw the endpoint in the PATHCOLOUR and set onEnd true.
  if(!input->active){
    if(active){  
      if(grid->get_type(gx, gy) == P_END){
        draw->colour_element(gx, gy, grid->path_colour);
        onEnd = true;
        return;
      }
    }
    return;
  }

  // update the input position and convert to grid position
  if(input->active){

    int cx = input->x;
    int cy = input->y;
    
    uint8_t gcx = round((cx - grid->x) / grid->spacing);
    gcx += gcx % 2;
    uint8_t gcy = round((cy - grid->y) / grid->spacing);
    gcy += gcy % 2;

    // if the snake was never activated we check for whih startpoint
    // the input might collide with and set the startpoint accordingly
    if(!active){
      for(int i = 0; i < grid->sy; i++){
        for(int j = 0; j < grid->sx; j++){
          if(grid->get_type(j, i) == P_STRT){
            if(Calc().collision(cx, cy, grid->gpx(j), grid->gpy(i), SNAPRADIUS)){
              active = true;
              set_start(j, i);
              draw->gfill_circle(gx, gy, STARTRADIUS, grid->path_colour);
              return;
            }
          }
        }
      }
    }

    // get the input. if its the same as the snakehead do nothing
    // else move into the direction of the touchposition.
    if(active){
      if(gcx == gx && gcy == gy){
        return;
      }
      move(direction(gcx, gcy));
    }
  }
}

// moves the snakehead in discrete vertex steps, if valid.
void Snake::move(uint8_t _dir){
  uint8_t tx = gx;
  uint8_t ty = gy;

  if(_dir == LEFT){
    tx -= 2;
  }

  if(_dir == RIGHT){
    tx += 2;
  }

  if(_dir == UP){
    ty -= 2;
  }

  if(_dir == DOWN){
    ty += 2;
  }

  // if there is no Cell or Path return without moving
  if(grid->get_type(tx, ty) == C_NONE){
    return;
  } 

  // if there is no edge between the vertices, you cant move
  if(!grid->walkable(gx + ((tx - gx) / 2), gy + ((ty - gy) / 2))){
    return;
  }
  
  // if there is no vertex you cant move.
  if(!grid->walkable(tx, ty)){
    return;
  }
  
  if(contains(tx, ty)){
    if(tx == get_gx_at(path_index - 1) && ty == get_gy_at(path_index - 1)){
    } else {
      return;
    }
  }

  gx = tx;
  gy = ty;
  x = grid->gpx(gx);
  y = grid->gpy(gy);
  path_handling();
  delay(100);
}

// returns pathpoints grid x pos at input index
uint8_t Snake::get_gx_at(uint8_t _index){
  return path[_index][0];
}

// returns pathpoints grid y pos at input index
uint8_t Snake::get_gy_at(uint8_t _index){
  return path[_index][1];
}

// returns last pathpoints grid x pos
uint8_t Snake::get_last_gx(){
  return path[path_index][0];
}

// returns last pathpoints grid y pos
uint8_t Snake::get_last_gy(){
  return path[path_index][1];
}

// adds or removes points from path and calls the path draw
void Snake::path_handling(){
  // takes the current positon and calculates distance to last position
  int d = Calc().distance(gx, gy, get_gx_at(path_index - 1), get_gy_at(path_index - 1));
  
  // if the distance is smaller than one vertex step, we are retracting the path. Else appending to it.
  if(d < 2 && path_index > 0){  
    draw->gline(get_last_gx(), get_last_gy(), get_gx_at(path_index - 1), get_gy_at(path_index - 1), PATHWIDTH, grid->grid_colour);
    draw->element(get_last_gx(), get_last_gy());
    path_index--;
  } else {
    path_index++;
    path[path_index][0] = gx;
    path[path_index][1] = gy;
  } 
  draw_path(grid->path_colour);
}

// draws the full path.
void Snake::draw_path(int _colour){
  // first the startpoint,
  draw->fill_circle(grid->gpx(get_gx_at(0)), grid->gpy(get_gy_at(0)), STARTRADIUS, _colour);

  // then the edgesegments, 
  for(int i = 1; i <= path_index; i++){
    draw->gline(get_gx_at(i), get_gy_at(i), get_gx_at(i - 1), get_gy_at(i - 1), PATHWIDTH, _colour);
  }

  // then the "head" as a radius.
  draw->fill_circle(x, y, PATHRADIUS, _colour);
}

// converts touchposition relative to headposition to direction
uint8_t Snake::direction(uint8_t gcx, uint8_t gcy){
  if(abs(gcx - gx) > abs(gcy - gy)){
    if(gcx > gx){
      return RIGHT;
    } else {
      return LEFT;
    }
  }
  if(abs(gcy - gy) != 0){
    if(gcy > gy){
      return DOWN;
    } else {
      return UP;
    }
  }
}

// looks through the drawn path and tells if it contains the input location
bool Snake::contains(uint8_t _gx, uint8_t _gy){
  // cell handling
  // every combination of uneven numbers is a cell. path cannot be on a cell.
  if((_gx % 2) == 1 && (_gy % 2) == 1){
    // draw->fillCircle(10 * _gx, 10 * _gy, 2, GREEN);  // for visual debugging
    return false;
  }

  // vertex handling
  // to check if its a vertex x and y are both 0 or 2 when modulo with 2; every uneven is between vertices
  if((_gx % 2) != 1 && (_gy % 2) != 1){
    if(contains_at(_gx, _gy) == 126){
      return false;
    } else {
      // draw->fillCircle(10 * _gx, 10 * _gy, 2, 0x1234); // for visual debugging
      return true;
    }
  }

  // edge handling
  // now we now its not a vertex and not a cell. either x or y needs to be 1 then when modulo with 2;
  // so we first check if the path runs through the vertex left to the current edge. If not we can 
  // already be sure there is no path. If path contains this point we need to check if either the previous
  // or the next pathpoint is the one right of the current edge. Thus (abs(x-1) - (abs+1)) should be one.
  if((_gx % 2) == 1){
    if(contains_at(_gx - 1, _gy) == 126){
      return false;
    } 
    if(abs(contains_at(_gx + 1, _gy) - contains_at(_gx - 1, _gy)) == 1){
      // draw->fillCircle(10 * _gx, 10 * _gy, 2, 0x5678); // for visual debugging
      return true;
    }
  } else {
    if(contains_at(_gx, _gy - 1) == 126){
      return false;
    } 
    if(abs(contains_at(_gx, _gy + 1) - contains_at(_gx, _gy - 1)) == 1){
      // draw->fillCircle(10 * _gx, 10 * _gy, 2, 0x5678); // for visual debugging
      return true;
    }
  }
  return false;
}

// Searches through the path to check if it contains the requested gridposition
uint8_t Snake::contains_at(uint8_t _gx, uint8_t _gy){
  for(int i = 0; i <= path_index; i++){
    if(path[i][0] == _gx && path[i][1] == _gy){
      return i;
    }   
  }
  // if not return 126 as "false" identifier, because false, aka 0 could also be an int.
  return 126;
}


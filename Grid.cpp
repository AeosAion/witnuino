#include "Grid.h"

Grid::Grid() {
}

Grid::Grid(uint8_t _level_index){
  level_index = _level_index;
}

bool Grid::setup() {
  load_map();
  calc_pos();
  return true;
}

// Reads the level file from the SD and creates gridmap
void Grid::load_map(){
  // INIT sd handling
  DataHandler dh = DataHandler();
  dh.setup();
  
  // SAVE FILE
  dh.open_txt_file("save");
  for(int i = 0; i < 3; i++){
    level_digits[i] = dh.read_next() - '0';
  }
  dh.close_file();

  // LEVEL FILE
  dh.open_txt_file(String(level_digits[0]) + String(level_digits[1]) + String(level_digits[2]));

  // uint8_t colour_index = dh.read_next();

  uint8_t colour_index = dh.read_next() - '0';
  colour_palette(colour_index);

  // bg_colour = AREA00_BG; // + colour_index;
  // grid_colour = AREA00_GC; // + colour_index;
  // path_colour = AREA00_PC; // + colour_index;   
  
  sx = dh.read_next() - '0';
  sy = dh.read_next() - '0';

  // skipping the "new line" chars
  if(dh.read_next() == 13){
    dh.read_next();
  }
  
  for (int8_t i = 0; i < sy; i++) {
    for (int8_t j = 0; j <= sx; j++) {
      uint8_t g = dh.read_next(); //

      if (j < sx) {
        elements_map[j][i] = g;
        // Serial.print(g);
      }
            
      if(g == 13){
        dh.read_next();
      }

      // Serial.println("");
    }
  }
  dh.close_file();  
}

void Grid::colour_palette(uint8_t index){
    bg_colour = AREA00_BG; // + colour_index;
    grid_colour = AREA00_GC; // + colour_index;
    path_colour = AREA00_PC; // + colour_index;    

  if( index ==  1){
    bg_colour = AREA01_BG; // + colour_index;
    grid_colour = AREA01_GC; // + colour_index;
    path_colour = AREA01_PC; // + colour_index;    
  }

  if( index ==  2){
    bg_colour = AREA02_BG; // + colour_index;
    grid_colour = AREA02_GC; // + colour_index;
    path_colour = AREA02_PC; // + colour_index;    
  }

  if( index ==  3){
    bg_colour = AREA03_BG; // + colour_index;
    grid_colour = AREA03_GC; // + colour_index;
    path_colour = AREA03_PC; // + colour_index;    
  }
}

// returns type of a gridpos
uint8_t Grid::get_type(uint8_t _gx, uint8_t _gy){
  if (_gx < 0 || _gy < 0 || _gx > sx - 1 || _gy > sy - 1) {
    return C_NONE;
  }
  return elements_map[_gx][_gy];
}

// Calculating the spacing and position based on the grid size
void Grid::calc_pos() {
  uint8_t spacingX, spacingY;

  if (sx != 1) {
    spacingX = (SCREENWIDTH - MARGIN) / (sx - 1);
  } else {
    spacingX = 0;
  }

  if (sy != 1) {
    spacingY = (SCREENHEIGHT - MARGIN) / (sy - 1);
  } else {
    spacingY = 0;
  }

  spacing = spacingX;

  if (spacingX > spacingY || sx == 1) {
    spacing = spacingY;
  }

  if (spacingY == 1) {
    spacing = spacingX;
  }

  int width = (spacing * (sx - 1));
  int height = (spacing * (sy - 1));

  x = (SCREENWIDTH - width) / 2;
  y = (SCREENHEIGHT - height) / 2;
}

bool Grid::walkable(uint8_t _gx, uint8_t _gy){
  uint8_t type = elements_map[_gx][_gy];
  if(type == P_STRT || type == P_END || type == P_VRTX || type == P_HORZ || type == P_HEX){
    return true;
  }
  return false;
}

// converts and returns grid x pos to pixel x pos
int Grid::gpx(uint8_t _gx){
  int x0 = (_gx * spacing) + x;
  return x0;
}

// converts grid y pos to pixel y pos
int Grid::gpy(uint8_t _gy){
  int y0 = (_gy * spacing) + y;
  return y0;
}
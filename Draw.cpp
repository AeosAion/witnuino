#include "Draw.h"

Draw::Draw(){
  
}

Draw::Draw(Grid* _grid){
  grid = _grid;
}

void Draw::setup(){
  backlighting(0);
  tft.begin();
  fill_screen(AREA00_BG);
  tft.setTextColor(BLACK);
}

// Set the backlighting to a brightness.
void Draw::backlighting(int brightness){
  pinMode(5, OUTPUT);
  analogWrite(5, brightness);
}

// Increase or decrease the backlighting recursively from a brightness to a brightness. 
void Draw::fade(uint8_t delay_time, int from_brightness, int to_brightness){
  if(from_brightness == to_brightness)
  {
    return;
  }
  if(from_brightness < to_brightness){
    from_brightness++;
  } else {
    from_brightness--;
  }

  analogWrite(5, from_brightness);  
  delay(delay_time);
  fade(delay_time, from_brightness, to_brightness);
}

// Fill the whole screen.
void Draw::fill_screen(int _colour){
  tft.fillScreen(_colour);  
}

// Fill circle based on pixel coordinates.
void Draw::fill_circle(int _x, int _y, int _r, int _colour){
  tft.fillCircle(_x, _y, _r, _colour);
}

// Line from grid coordinate to grid coordinate with width. VERTICAL OR HORIZONTAL ONLY!!
void Draw::gline(uint8_t _gx0, uint8_t _gy0, uint8_t _gx1, uint8_t _gy1, int _width, int _colour){
  int length;
  int widthOffset = _width / 2;

  int x0 = grid->gpx(_gx0);
  int y0 = grid->gpy(_gy0);

  int x1 = grid->gpx(_gx1);
  int y1 = grid->gpy(_gy1);

  int x2 = x0;
  int y2 = y0;

  if (y0 == y1) {
    length = abs(x0 - x1);
    if (x0 > x1) {
      x2 = x1;
    }
    for (int8_t i = 0; i < widthOffset; i++) {
      tft.drawFastHLine(x2, y2 + i, length, _colour);
      tft.drawFastHLine(x2, y2 - i, length, _colour);    
      }
  }

  if (x0 == x1) {
    length = abs(y0 - y1);
    if (y0 > y1) {
      y2 = y1;
    }
    for (int8_t i = 0; i < widthOffset; i++) {
      tft.drawFastVLine(x2 + i, y2, length, _colour);
      tft.drawFastVLine(x2 - i, y2, length, _colour);
    }
  }
}

// Line from pixel coordinate to pixel coordinate with width. VERTICAL OR HORIZONTAL ONLY!!
void Draw::line(int _x0, int _y0, int _x1, int _y1, int _width, int _colour){
  int length;
  int widthOffset = _width / 2;

  int x2 = _x0;
  int y2 = _y0;

  if (_y0 == _y1) {
    length = abs(_x0 - _x1);
    if (_x0 > _x1) {
      x2 = _x1;
    }
    for (int8_t i = 0; i < widthOffset; i++) {
      tft.drawFastHLine(x2, y2 + i, length, _colour);
      tft.drawFastHLine(x2, y2 - i, length, _colour);    
      }
  }

  if (_x0 == _x1) {
    length = abs(_y0 - _y1);
    if (_y0 > _y1) {
      y2 = _y1;
    }
    for (int8_t i = 0; i < widthOffset; i++) {
      tft.drawFastVLine(x2 + i, y2, length, _colour);
      tft.drawFastVLine(x2 - i, y2, length, _colour);
    }
  }
}

// Fill circle based on grid coordinates.
void Draw::gfill_circle(uint8_t _gx, uint8_t _gy, int _r, int _colour){
  int x0 = grid->gpx(_gx);
  int y0 = grid->gpy(_gy);
  fill_circle(x0, y0, _r, _colour);
}

// Draw the full puzzle.
void Draw::puzzle(){
  edges();
  elements();
}

// Draw all edges. Basically the walkable grid.
void Draw::edges(){
  for (int8_t i = 0; i < grid->sy; i++) {
    for (int8_t j = 0; j < grid->sx; j++) {
      uint8_t type = grid->get_type(j, i); 
      if(type == P_STRT || type == P_END || type == P_VRTX || type == P_HEX || type == P_HORZ){
        element(j, i);
      }
    }
  }
}

// Draw the element at grid coordinate.
void Draw::element(uint8_t _gx, uint8_t _gy) {
  uint8_t type = grid->get_type(_gx, _gy);
  // Vertex
  if (type == P_VRTX || type == P_HEX) {
    gfill_circle(_gx, _gy, PATHRADIUS, grid->grid_colour);
  }

  // Path
  if (type == P_HORZ) {  
    for (int8_t i = -1; i < 2; i++) {
      uint8_t nt = grid->get_type(_gx + i, _gy);
      if (nt == P_VRTX || nt == P_STRT || nt == P_END || nt == P_HORZ || nt == P_HEX) {
        gline(_gx, _gy, _gx + i, _gy, PATHWIDTH, grid->grid_colour);
      }
      nt = grid->get_type(_gx, _gy + 1);
      if (nt == P_VRTX || nt == P_STRT || nt == P_END || nt == P_HORZ || nt == P_HEX) {
        gline(_gx, _gy, _gx, _gy + i, PATHWIDTH, grid->grid_colour);
      }
    }
  }
 
  // Startpoint
  if (type == P_STRT) {  
    gfill_circle(_gx, _gy, STARTRADIUS, grid->grid_colour);
  }

  // Endpoint
  if (type == P_END) {  
    endcap(_gx, _gy, grid->grid_colour);
  }

  // Hex
  if (type == P_HEX){     
    hex(_gx, _gy, HEXCOLOUR);
  }

  // Triangle
  if (type >= C_TRI1 && type <= C_TRI3) {  // triangle tile
    triangle(_gx, _gy, type - (C_TRI1 - 1), TRICOLOUR);
  }

  // Square
  if (type == C_SQ_B){
    fill_circle(grid->gpx(_gx), grid->gpy(_gy), grid->spacing / 4, BLACK);
  }
  if (type == C_SQ_W){
    fill_circle(grid->gpx(_gx), grid->gpy(_gy), grid->spacing / 4, WHITE);
  }

  // Sun
  if (type == C_SUN_B){
    sun(_gx, _gy, BLACK);
  }

  if (type == C_SUN_W){
    sun(_gx, _gy, WHITE);
  }
}

// Colour the element at grid coordinate. Mainly for flashing invalids.
void Draw::colour_element(uint8_t _gx, uint8_t _gy, int _colour){
  uint8_t type = grid->get_type(_gx, _gy);  
  
  // ENDCAP
  if (type == P_END){     
    endcap(_gx, _gy, _colour);
  }
  
  // HEX
  if (type == P_HEX){     
    hex(_gx, _gy, _colour);
  }

  // TRIANGLE
  if (type >= C_TRI1 && type <= C_TRI3) {  // triangle tile
    triangle(_gx, _gy, type - (C_TRI1 - 1), _colour);
  }

  // SQUARE
  if (type >= C_SQ_B && type <= C_SQ_W){
    fill_circle(grid->gpx(_gx), grid->gpy(_gy), grid->spacing / 4, _colour);
  }

  // SUN
  if (type >= C_SUN_B && type <= C_SUN_W){
    sun(_gx, _gy, _colour);
  }
}

// Draw the endcap in the right direction.
void Draw::endcap(uint8_t _gx, uint8_t _gy, int _colour){
  int from_x = grid->gpx(_gx);
  int from_y = grid->gpy(_gy);
  int to_x = from_x;
  int to_y = from_y; 
  int offset = 14;

  if(_gy == 0){
    to_y -= offset;
    goto draw;
  }

  if(_gy == (grid->sy - 1)){
    to_y += offset;
    goto draw;
  }

  if(_gx == 0){
    to_x -= offset;
    goto draw;
  }

  if(_gx == (grid->sx - 1)){
    to_x += offset;
    goto draw;
  }

  draw:
  fill_circle(from_x, from_y, PATHRADIUS, _colour);
  line(from_x, from_y, to_x, to_y, PATHWIDTH, _colour);
  fill_circle(to_x, to_y, PATHRADIUS, _colour);
}

// Draw all puzzle elements, except for the edges, so the hexs are drawn on top.
void Draw::elements(){
  for (int8_t i = 0; i < grid->sy; i++) {
    for (int8_t j = 0; j < grid->sx; j++) {
      if(grid->get_type(j, i) != P_HORZ){
        element(j, i);
      }  
    }
  } 
}

// Draw a triangle puzzle element. 
void Draw::triangle(uint8_t _gx, uint8_t _gy, uint8_t _amnt, int _colour) {
  
  // d = distance to next triangle (if multiple, based on _amnt) and r = radius of a triangle
  int d = round(grid->spacing / 4);
  int r = d - (d / 4);

  int pix_x = grid->gpx(_gx) - ((_amnt - 1) * d) - r;
  int pix_y = grid->gpy(_gy) + r;

  for (uint8_t i = 0; i < _amnt; i++) {
    int offset = (i * (2 * d));
    for (int i = 0; i < (r * 2); i++) {
      tft.drawFastHLine(pix_x + (i / 2) + offset, pix_y - i, (r * 2) - i, _colour);
    }
  }
}

// Draw hexagon puzzle element.
void Draw::hex(uint8_t _gx, uint8_t _gy, int _colour){
  int r = PATHRADIUS - 2;
  int pix_x = grid->gpx(_gx) - r;
  int pix_y = grid->gpy(_gy);

  for(uint8_t i = 0; i < r; i++){
    tft.drawFastHLine(pix_x + (i / 2), pix_y + i, (r - (i / 2)) * 2, _colour);
    tft.drawFastHLine(pix_x + (i / 2), pix_y - i, (r - (i / 2)) * 2, _colour);
  }
} 

void Draw::sun(uint8_t _gx, uint8_t _gy, int _colour){
  int r = round(grid->spacing / 3);
  long r2 = round(sqrt(pow(r, 2) / 2));  // sqrt(pow(a, 2) * 2);
  int pix_x = grid->gpx(_gx);
  int pix_y = grid->gpy(_gy);
  
  // simple square
  for(uint8_t i = 0; i < r2; i ++){
    tft.drawFastHLine(pix_x - r2, pix_y + i, r2 * 2, _colour);
    tft.drawFastHLine(pix_x - r2, pix_y - i, r2 * 2, _colour);
  }
  
  // 45° degree tilted square
  for(uint8_t i = 0; i < r; i ++){ 
    tft.drawFastHLine(pix_x - r + i, pix_y + i, (r * 2) - (i * 2), _colour);
    tft.drawFastHLine(pix_x - r + i, pix_y - i, (r * 2) - (i * 2), _colour);
  }
}

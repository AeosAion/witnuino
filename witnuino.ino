#include "Globals.h"
#include "Grid.h"
#include "Input.h"
#include "Snake.h"
#include "Solver.h"
#include "Draw.h"

// GRID contains grid information
Grid grid;

// INPUT captues the touch-input
Input input;

// SNAKE is the user-drawn path
Snake snake;

// DRAW draws to the TFT
Draw draw;

uint8_t lvl = 0;

void setup() {
  // Serial.begin(9600);  // for now deactivated to save program storage space
  
  // start up the TFT
  draw = Draw(&grid);
  draw.setup();

  // start up the Touch
  input = Input();
  input.setup();

  // setup the level and fade display in
  setup_level(1);
  draw.fade(1, 0, 120);
}

void setup_level(bool draw_bg){ 
  // instantiate and setup GRID from the specified level file.
  // TODO: get level index from save.txt from SD instead.
  grid = Grid(lvl);  
  grid.setup();

  // instantiate and setup SNAKE for the grid.
  snake = Snake(&grid, &input, &draw);
  snake.setup();
  
  if(draw_bg){
    draw.fill_screen(grid.bg_colour);
  }

  // draw the current puzzle to the TFT
  draw.puzzle();
}

void loop() {
  // INPUT and SNAKE need to be updated every cycle...
  // in case the user made any inputs.
  input.update();
  snake.update();

  // if the player stopped at the endpoint...
  // we instantiate a SOLVER to check the solution for validity.
  if(snake.onEnd){
    Solver solver = Solver(&snake, &grid, &draw);
    if(solver.validate()){
      lvl++;
      delay(1000);
      setup_level(1);
    } else {
      solver.flash_invalids();
      setup_level(0);
    }
  }
}




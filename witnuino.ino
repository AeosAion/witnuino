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

void setup() {
  // Serial.begin(9600);  // for now deactivated to save program storage space
  
  // start up the TFT
  draw = Draw(&grid);
  draw.setup();

  // start up the Touch
  input = Input();
  input.setup();

  // setup the level and fade display in
  setup_level();
}

void setup_level(){ 
  // instantiate and setup GRID from the specified level file.
  grid = Grid();  
  grid.setup();

  // instantiate and setup SNAKE for the grid.
  snake = Snake(&grid, &input, &draw);
  snake.setup();
  
  draw.fill_screen(grid.bg_colour);

  // draw the current puzzle to the TFT
  draw.puzzle();
  draw.fade(1, 0, 120);
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
      delay(1000);
      draw.fade(1, 120, 0);
      delay(100);
    } else {
      solver.flash_invalids();
      draw.fade(1, 120, 0);
    }
    setup_level();
  }
}




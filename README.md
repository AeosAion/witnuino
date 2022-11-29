# Witnuino
## The puzzle language of "The Witness" adapted for Arduino

This is an adoption of the panel puzzles in the witness for the Arduino. I'm using the Adafruit capacitive 2.8" TFT. The main challenge for me right now is to fit the whole code on the flash storage of an Arduino UNO. Memory is currently no issue. 

Functional Puzzle Elements:
- Mazes
- Hexagons
- Triangles
- Squares


### witnuino.ino
instantiates everything, sets up levels and updates on every loop.

### DataHandler.h
handles communication with SD-card and reads data from .txt files.

### Draw.h 
handles the communication with the TFT display and draws on it. Also holds some functions for specific drawing, like lines from a grid-coordinate to another.

### Grid.h 
creates an array of puzzle elements at their respective grid-coordinates.

### Globals.h 
wait, wait, wait... Its just some #define definitions that every class can include if neccessary. Maybe this is still not a nice style to do this?

### Input.h 
handles the communication with the touchscreen and holds the user touch position.

### Snake.h 
the snake / path the user can interact with and draw. 

### Solver.h
validates the solution that was input and gathers invalid elements if there are any.


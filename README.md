# Witnuino
## The puzzle language of "The Witness" adapted for Arduino

This is an adoption of the panel puzzles in the witness for the Arduino. I'm using the Adafruit capacitive 2.8" TFT for input and displaying and the SD reader that comes with it for saving the level data. The code reads the level data from the SD, creates the puzzle from that data, lets the user interact with the path and finally returns the validity of the solution that was input. I'm really excited for this current version since a lot of the puzzle elements already fully work. There are a few displaying bugs to be fixed and style improvements to be made and hopefully, most importantly, more puzzle rules to be added. The main challenge for me right now is to fit the whole code on the flash storage of an Arduino UNO. Memory/ RAM is currently no issue. Almost everything is commented in the code though feel free to ask if something is unclear!

Functional Puzzle Elements:
- Mazes
- Hexagons
- Triangles
- Squares

### /witnuino (folder)
this goes to the root folder of the SD card. The code will search for this folder and will open it to look for the level and save files. The level files are currently named "x.txt" (with x being the levelnumber) for ease of use and to reduce the lines needed to locate them. The "save.txt" is not used yet due to storage shortcomings yet the system will write the last loaded level index to it, so we can load that when we restart the system.

### witnuino.ino
this is the main sketch. It instantiates everything, sets up levels and updates the input and snake on every loop.

### DataHandler.h
handles communication with SD-card and reads data from the .txt files.

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

### Level Files
The level files are coded in a quite simple fashion. ASCII coding is used to be able to visually edit and build the levels in a .txt file on your computer. This is the table of equivalent puzzle/ grid element to each character.


| **char** | **element**  | **bit** |   |   |
|----------|--------------|---------|---|---|
| s        | startpoint   | 115     |   |   |
| e        | endcap       | 101     |   |   |
| o        | vertex       | 43      |   |   |
| +        | vertex hex   | 44      |   |   |
| -        | edge         | 45      |   |   |
| .        | edge hex     | 46      |   |   |
| /        | edge broken  | 47      |   |   |
| ---      | ---          | ---     |   |   |
| x        | empty        | 120     |   |   |
| X        | empty        | 88      |   |   |
| ---      | ---          | ---     |   |   |
| A        | Triangle 1   | 65      |   |   |
| B        | Triangle 2   | 66      |   |   |
| C        | Triangle 3   | 67      |   |   |
| ---      | ---          | ---     |   |   |
| D        | Square Black | 68      |   |   |
| E        | Square White | 69      |   |   |
| F        | Square Red   | 70      |   |   |
| G        | Square Green | 71      |   |   |
| H        | Square Blue  | 72      |   |   |
| ---      | ---          | ---     |   |   |
| I        | Sun Black    | 73      |   |   |
| J        | Sun White    | 74      |   |   |
| K        | Sun Red      | 75      |   |   |
| L        | Sun Green    | 76      |   |   |
| M        | Sun Blue     | 77      |   |   |



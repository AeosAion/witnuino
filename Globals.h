#include "Arduino.h"
#include "Calc.h"

#define SCREENWIDTH 240
#define SCREENHEIGHT 320

// basic colours
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define YELLOW   0xFFE0 

// essential element colours palette
#define BLACK    0x0000
#define WHITE    0xFFFF
#define CYAN     0x05db
#define MAGENTA  0xf88e
#define ORANGE   0xf340 // 0xeae0 // 0xf3e0 // older palette
#define GREY     0xBDF7

// area colours

// YELLOW & WHITE
#define AREA00  48
#define AREA00_BG 0xED80
#define AREA00_GC 0x10a2
#define AREA00_PC 0xFF32

// BLUE & BLUE
#define AREA01 49
#define AREA01_BG 0x59F1 // 0x398F // 0x198C
#define AREA01_GC 0x0042
#define AREA01_PC 0xB69F 

// GREY & ORANGE
#define AREA02  50
#define AREA02_BG 0x5b0b
#define AREA02_GC 0x10a2
#define AREA02_PC 0xED80

// BROWN & BLUE
#define AREA03  51
#define AREA03_BG 0x6163
#define AREA03_GC BLACK
#define AREA03_PC 0xa4b6

// puzzle element types
// EDGES
#define P_VRTX    (uint8_t)111
#define P_HEX     (uint8_t)43
#define P_HORZ    (uint8_t)45
#define P_BROKEN  (uint8_t)46
#define P_STRT    (uint8_t)115
#define P_END     (uint8_t)101

#define C_NONE    (uint8_t)120

// TRIANGLES
#define C_TRI1    (uint8_t)65      
#define C_TRI2    (uint8_t)66       
#define C_TRI3    (uint8_t)67

// SQUARES
#define C_SQ_B    (uint8_t)68
#define C_SQ_W    (uint8_t)69
#define C_SQ_CYAN (uint8_t)70
#define C_SQ_MAGENTA  (uint8_t)71
#define C_SQ_ORANGE (uint8_t)72

// SUNS
#define C_SUN_B   (uint8_t)73
#define C_SUN_W   (uint8_t)74
#define C_SUN_CYAN (uint8_t)75
#define C_SUN_MAGENTA (uint8_t)76
#define C_SUN_ORANGE (uint8_t)77

// path + grid definitions
#define PATHWIDTH   (uint8_t)21
#define PATHRADIUS  (uint8_t)9
#define SNAPRADIUS  (uint8_t)20
#define STARTRADIUS (uint8_t)18
#define MARGIN      (uint8_t)70

// movement
#define LEFT  (uint8_t)0
#define RIGHT (uint8_t)1
#define UP    (uint8_t)2
#define DOWN  (uint8_t)3

#ifndef DataHandler_h
#define DataHandler_h

#include "Arduino.h"
#include "SdFat.h"

#define SD_CS 4

class DataHandler{
  public: 
    DataHandler();
   
    // init SD, navigate to game dir.
    bool setup();                     
    
    // open specified .txt to File file
    bool open_txt_file(String name);
    
    // close current open File file   
    void close_file();        

    // read and return the next char from .txt        
    uint8_t read_next();
    
    // TODO: NOT TESTED!! write value to file (we need to write the level index to save.txt).              
    bool write_next(uint8_t value);   

  private:
    SdFat sd;
    File file;
};

#endif
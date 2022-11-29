#include "DataHandler.h"

DataHandler::DataHandler(){

}

// init SD, navigate to game dir.
bool DataHandler::setup(){

  // Init the SD. 
  if (!sd.begin(SD_CS, SD_SCK_MHZ(25))) { 
    // Serial.println(("SD failed."));
    return false;
    for (;;)
      // error
      ;                                    
  }
  // Serial.println("SD init.");
  
  // navigate to game directory located in root
  if(sd.chdir("/witnuino")){
    // Serial.println("Navigated to game directory.");
  } else {
    // Serial.print("ERR404");
  }
  
  return true;
}

// open specified .txt to File file
bool DataHandler::open_txt_file(String name){
  String fnameS = name + ".txt";
  uint8_t fnl = fnameS.length() + 1;
  char fname[fnl];
  fnameS.toCharArray(fname, fnl);

  if(!file.open(fname, O_RDWR)){
    return false;
  }

  return true;
}

// close current open File file 
void DataHandler::close_file(){
  file.close();
}

// read and return the next char from .txt
uint8_t DataHandler::read_next(){
  char c = file.read();
  return c;
}

// TODO: NOT TESTED!! write value to file (we need to write the level index to save.txt).
bool DataHandler::write_next(uint8_t value){
  file.write(value);
  return;
}



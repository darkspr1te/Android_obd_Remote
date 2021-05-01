#define HUnit_mod 
#include <JunsunPSACANRemote.h>

#define IO_HUenable       //Poweron
#define IO_HUrear         //rear gear signal
#define IO_HUbrake        //handbrake signal
#define IO_HUill          //Illimation signal

#define SERIAL_SPEED    19200
JunsunPSACANRemote* remote;

void setupHUnit() {
  setup_IO();
  Serial.begin(SERIAL_SPEED);  
  remote = new JunsunPSACANRemote(Serial);
}

void HUnit() {
  
}


void setup_IO() {
  pinMode(IO_HUenable,OUTPUT);
  pinMode(IO_HUrear,OUTPUT);
  pinMode(IO_HUbrake,OUTPUT);
  pinMode(IO_HUill,OUTPUT);
}

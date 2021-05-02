#define HUnit_mod 
#include <JunsunPSACANRemote.h>

#define IO_HUenable       //Car ignition signal
#define IO_HUrear         //reverse gear signal
#define IO_HUbrake        //handbrake signal
#define IO_HUill          //Illumimation signal

#define SERIAL_SPEED    19200
JunsunPSACANRemote* remote;

void setupHUnit() {
  setup_IO();
  Serial.begin(SERIAL_SPEED);  
  remote = new JunsunPSACANRemote(Serial);
}

void HUnit() {
  if (Car.H_UPD_IO) update_IO();
}


void setup_IO() {
  pinMode(IO_HUenable,OUTPUT);
  pinMode(IO_HUrear,OUTPUT);
  pinMode(IO_HUbrake,OUTPUT);
  pinMode(IO_HUill,OUTPUT);
  update_IO();
}

void update_IO() {
  digitalWrite(IO_HUenable, ign); // put !ign if nedds to reversed
  digitalWrite(IO_HUrear, rear); // put !rear if nedds to reversed
  digitalWrite(IO_HUbrake, brake); // put !brake if nedds to reversed
  digitalWrite(IO_HUill, ill);// put !ill if nedds to reversed
  Car.H_UPD_IO=false; //updated
}

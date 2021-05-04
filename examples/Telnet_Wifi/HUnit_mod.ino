#define HUnit_mod 
#include <JunsunPSACANRemote.h>

#define IO_HUenable      7 //Car ignition signal
#define IO_HUrear        7 //reverse gear signal
#define IO_HUbrake       7 //handbrake signal
#define IO_HUill         7 //Illumimation signal

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
  digitalWrite(IO_HUenable, Car.ign); // put !ign if nedds to reversed
  digitalWrite(IO_HUrear, Car.rear); // put !rear if nedds to reversed
  digitalWrite(IO_HUbrake, Car.brake); // put !brake if nedds to reversed
  digitalWrite(IO_HUill, Car.ill);// put !ill if nedds to reversed
  Car.H_UPD_IO=false; //updated
}

void HU_button(byte button) {
  uint8_t but[] = { 0x02, button};
  remote->SendData(but, 2);
}

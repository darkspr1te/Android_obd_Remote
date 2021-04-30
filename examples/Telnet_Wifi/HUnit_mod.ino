#define HUnit_mod 
#include <JunsunPSACANRemote.h>

#define SERIAL_SPEED    19200
JunsunPSACANRemote* remote;

Void setupHUnit() {
  Serial.begin(SERIAL_SPEED);  
  remote = new JunsunPSACANRemote(Serial);
}

Void HUnit() {
  
}

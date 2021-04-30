#include <JunsunPSACANRemote.h>

#define SERIAL_SPEED    19200

#define WIFI_SSID       "MY SSID"
#define WIFI_PASSWORD   "MY PASS"

JunsunPSACANRemote* remote;

void setup() {
  #if defined(HUnit_mod) 
  setupHUnit();
  #endif
  
  Serial.begin(SERIAL_SPEED);  
  remote = new JunsunPSACANRemote(Serial);
  #if defined(canbus1_mod) 
  #endif
  #if defined(canbus2_mod) 
  #endif
  #if defined(Perso_mod)
  #endif
  #if defined(telnet_mod) 
  setupTelnet();
  #endif
  #if defined(websrv_mod) 
  #endif
  
}

void loop() {
  #if defined(HUnit_mod) 
  HUnit();
  #endif
  #if defined(canbus1_mod) 
  #endif
  #if defined(canbus2_mod) 
  #endif
  #if defined(Perso_mod)
  #endif
  #if defined(telnet_mod) 
  telnet();
  }
  #endif
  #if defined(websrv_mod) 
  #endif
  
}


void errorMsg(String error, bool restart = true) {
  if (restart) {
    delay(2000);
    ESP.restart();
    delay(2000);
  }
}

#include <JunsunPSACANRemote.h>

#define SERIAL_SPEED    19200

#define WIFI_SSID       "MY SSID"
#define WIFI_PASSWORD   "MY PASS"

JunsunPSACANRemote* remote;

void errorMsg(String error, bool restart = true) {
  if (restart) {
    delay(2000);
    ESP.restart();
    delay(2000);
  }
}

void setup() {
  Serial.begin(SERIAL_SPEED);  
  remote = new JunsunPSACANRemote(Serial);
  
    #if defined(telnet_mod) 
    setupTelnet();
    #endif
  
}

void loop() {
  #if defined(telnet_mod) 
  telnet.loop();
  if (Serial.available()) {
    telnet.print(Serial.read());
  }
  #endif
  #if defined(websrv_mod) 
  #endif
  #if defined(canbus1_mod) 
  #endif
  #if defined(canbus2_mod) 
  #endif
  #if defined(Perso_mod)
  #endif
}

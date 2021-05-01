// remove the modules files you don't use
// it will compile without them and free flash space from MCU
// For exemple if you dont want use WIFI remove WIFI_mod.ino from your sketch folder and wifi will disabled

// wifi options (SSID, Password, max rery, mdns hostname are in "WIFI_mod.ino"

#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif

#define FILESYSTEM SPIFFS

#define IO_HUenable       //Poweron
#define IO_HUrear         //rear gear signal
#define IO_HUbrake        //handbrake signal
#define IO_HUill          //Illimation signal

void setup() {
    setup_IO();
  #if defined(HUnit_mod) 
    setupHUnit();
  #endif
  #if defined(canbus1_mod) 
  #endif
  #if defined(canbus2_mod) 
  #endif
  #if defined(Perso_mod)
  #endif
  #if defined(WIFI_mod) 
    wifi_conf();
  #endif
  FILESYSTEM.begin();
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
  #if defined(WIFI_mod) 
    WIFI_loop();
  #endif
}
void setup_IO() {
  pinMode(IO_HUenable,OUTPUT);
  pinMode(IO_HUrear,OUTPUT);
  pinMode(IO_HUbrake,OUTPUT);
  pinMode(IO_HUill,OUTPUT);
}

void errorMsg(String error, bool restart = true) {
  if (restart) {
    delay(2000);
    ESP.restart();
    delay(2000);
  }
}

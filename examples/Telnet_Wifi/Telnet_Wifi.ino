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

struct Car_data {
    uint16_t I_speed, I_rpm; // odometer info
    bool ign,ill,rear,brake; // I/O status
    byte AC_L,AC_R,AC_FAN; // A/C values
    bool AC_DOWN,AC_UP,AC_FRONT,AC_Recycle,AC_WINDSH,AC_AUTO,AC_COMP,AC_OFF; 
    bool DOOR_FL,DOOR_FR,DOOR_RL,DOOR_RR,DOOR_TR; //door status
    bool H_UPD_AC,H_UPD_I,H_UPD_IO,H_UPD_DOOR; //update flag
};

Car_data Car;


void setup() {
  #if defined(HUnit_mod) 
    setupHUnit();
  #endif
  #if defined(canbus1_mod)
    CAN1_setup();
  #endif
  #if defined(canbus2_mod)
    CAN2_setup();
  #endif
  #if defined(Perso_mod)
    Perso_setup();
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
    CAN1_loop();
  #endif
  #if defined(canbus2_mod)
    CAN2_loop();
  #endif
  #if defined(Perso_mod)
    Perso_loop();
  #endif
  #if defined(WIFI_mod) 
    WIFI_loop();
  #endif
}

void errorMsg(String error, bool restart = true) {
  if (restart) {
    delay(2000);
    ESP.restart();
    delay(2000);
  }
}

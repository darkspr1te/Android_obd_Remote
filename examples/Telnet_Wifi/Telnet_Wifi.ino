#include <JunsunPSACANRemote.h>

#define SERIAL_SPEED    19200

#define WIFI_SSID       "MY SSID"
#define WIFI_PASSWORD   "MY PASS"

IPAddress ip;

bool isConnected() {
  return (WiFi.status() == WL_CONNECTED);
}

bool connectToWiFi(const char* ssid, const char* password, int max_tries = 20, int pause = 500) {
  int i = 0;
  WiFi.mode(WIFI_STA);
  #if defined(ARDUINO_ARCH_ESP8266)
    WiFi.forceSleepWake();
    delay(200);
  #endif
  WiFi.begin(ssid, password);
  do {
    delay(pause);
  } while (!isConnected() || i++ < max_tries);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  return isConnected();
}


void errorMsg(String error, bool restart = true) {
  if (restart) {
    delay(2000);
    ESP.restart();
    delay(2000);
  }
}

void setup() {
  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  
  remote = new JunsunPSACANRemote(Serial);
  
  if (isConnected()) {
    ip = WiFi.localIP();
    #if defined(telnet_mod) 
    setupTelnet();
    #endif
  }
}

void loop() {
  #if defined(telnet_mod) 
  telnet.loop();
  if (Serial.available()) {
    telnet.print(Serial.read());
  }
  #endif
}

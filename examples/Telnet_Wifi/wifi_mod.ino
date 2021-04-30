#define WIFI_mod 

#define HostName Canbox

#define max_tries 10

#include <WiFi.h>
#include <ESPmDNS.h>

#define WIFI_SSID       "MY SSID"
#define WIFI_PASSWORD   "MY PASS"

IPAddress ip;

void WIFI_loop() {
  #if defined(telnet_mod) 
  telnet();
  #endif
  #if defined(websrv_mod) 
  websrv();
  #endif
}

bool isConnected() {
  return (WiFi.status() == WL_CONNECTED);
}

bool connectToWiFi(const char* ssid, const char* password, int max_tries = 20, int pause = 500) {
  byte i = 0;
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

void start_mdns_service()     //initialize mDNS service
{
    esp_err_t err = mdns_init();
    if (err) {
        printf("MDNS Init failed: %d\n", err);
        return;
    }

    mdns_hostname_set("HostName");
    mdns_instance_name_set("HostName");
}

void wifi_conf() {  
  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
      if (isConnected()) {
      ip = WiFi.localIP();
    }

  start_mdns_service();
  #if defined(telnet_mod) 
  setupTelnet();
  #endif
  #if defined(websrv_mod) 
  setupWebServer();
  #endif
}

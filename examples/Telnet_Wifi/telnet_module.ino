#define telnet_mod 
#include "ESPTelnet.h"          // https://github.com/LennartHennigs/ESPTelnet

ESPTelnet telnet;

void telent_conf() {      
  telnet.onConnect(onTelnetConnect);
  telnet.onConnectionAttempt(onTelnetConnectionAttempt);
  telnet.onReconnect(onTelnetReconnect);
  telnet.onDisconnect(onTelnetDisconnect);

  telnet.onInputReceived([](String str) {
    if (str == "b:source") {
      remote->SendButtonCode(Source);
      telnet.println("CON:>>HU Source");
    }
    if (str == "b:vol+") {
      remote->SendButtonCode(VolumeUp);
      telnet.println("CON:>>HU Vol+");
    }
    if (str == "b:vol-") {
      remote->SendButtonCode(VolumeDown);
      telnet.println("CON:>>HU Vol-");
    }
    if (str == "?") { //help 
      telnet.println("Commands :\n b:<value> Send <value> button to Head Unit use ?b: to see available values");
      telnet.println("r:<value> Send raw <value> Head Unit (start and chekcsum will be added) use ?r: for help");
      telnet.println("t:<number> <value> Send trip data to Head Unit use ?t: for help");
      telnet.println("a:<values> Send A/C data to Head Unit use ?a: for help");  
      telnet.println("hm:<value> filter received data by HU by function ");
      telnet.println("Hm:<value> filter emmited data by HU by function ");
      telnet.println("cm:<value> filter received data by CAN by function ");
      telnet.println("Cm:<value> filter emmited data by CAN by function ");
    }
    if (str == "?b:") { //help 
      telnet.println("Possible values :\n");
      telnet.println("source vol+ vol-");
    }
  });
}
    


void onTelnetConnect(String ip) {
  telnet.println("\nWelcome " + telnet.getIP());
  telnet.println("(Use ? for help)");
}

void onTelnetDisconnect(String ip) {
}

void onTelnetReconnect(String ip) {
}

void onTelnetConnectionAttempt(String ip) {
}

void setupTelnet() {
  telent_conf();
  if (telnet.begin()) {
  }
}

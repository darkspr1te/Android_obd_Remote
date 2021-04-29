#define telnet_mod


#include "ESPTelnet.h"          // https://github.com/LennartHennigs/ESPTelnet
ESPTelnet telnet;


void setupTelnet() {  
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
      telnet.println("Commands :\r\n b:<value> Send <value> button to Head Unit use ?b: to see available values");
      telnet.println("r:<value> Send raw <value> Head Unit (start and chekcsum will be added) use ?r: for help");
    }
    if (str == "?b:") { //help 
      telnet.println("Possible values :\r\n");
      telnet.println("source vol+ vol-");
    }
    
    
    
  });

  if (telnet.begin()) {
  }
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

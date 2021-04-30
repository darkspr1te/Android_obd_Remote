#include <WiFiClient.h>

WiFiServer server(80);

Void websrv() {
WiFiClient client = server.available();
    if (!client) {
        return;
    }
 /*while(client.connected() && !client.available()){
        delay(1);
    }
*/
    String req = client.readStringUntil('\r');
    req = req.substring(addr_start + 1, addr_end);
    String s;

    if (req == "/")
    {
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP32";
        s += "</html>\r\n\r\n";
    }
    else
    {
        s = "HTTP/1.1 404 Not Found\r\n\r\n";
    }
    client.print(s);
    client.stop();
}

Void setupWebServer() {
 server.begin();
}

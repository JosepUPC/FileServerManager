#include <Arduino.h>
#include "FS.h"
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

const char* ssid = "MIWIFI_2G_sfhx";
const char* password = "gJmjjuNe";

WebServer server(80); // Create a WebServer instance listening on port 80

void handleRoot() {
    // Open the index.html file
    File file = SPIFFS.open("/index.html", "r");
    if (file) {
        server.streamFile(file, "text/html");
        file.close();
    } else {
        server.send(404, "text/plain", "File Not Found");
    }
}
String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>Mi primer archivo enviado?</h1>\
</body>\
</html>";

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS initialization failed!");
        return;
    }
    File file = SPIFFS.open("/index.html", "w");
    if (file) {
        file.printf("%s", HTML);
        file.close();
    } else {
        server.send(404, "text/plain", "File Not Found");
    }
    Serial.println("SPIFFS initialized successfully!");

    // Route for root URI
    server.on("/", HTTP_GET, handleRoot);

    // Start the server
    server.begin();
    Serial.println("Server started");

    Serial.print("Server IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    server.handleClient(); // Handle client requests
}
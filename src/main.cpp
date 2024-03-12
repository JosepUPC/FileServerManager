#include <Arduino.h>
#include <WebServer.h>
#include <SPIFFS.h>

const char* ssid = "MIWIFI_2G_sfhx";
const char* password = "gJmjjuNe";

WebServer server(80); // Create a WebServer instance listening on port 80

String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>Mi primer archivo enviado?</h1>\
</body>\
</html>";

void handleClient() {
    // Open the index.html file
    File file = SPIFFS.open("/index.html", "r");
    if (file) {
        server.streamFile(file, HTML);
        file.close();
    } else {
        server.send(404, "text/plain", "File Not Found");
    }
}

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
    Serial.println("SPIFFS initialized successfully!");

    // Route for root URI
    server.on("/", HTTP_GET, handleClient);

    // Start the server
    server.begin();
    Serial.println("Server started");

    Serial.print("Server IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    server.handleClient(); // Handle client requests
}
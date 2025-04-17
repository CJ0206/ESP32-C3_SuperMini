#include "WiFi.h"

const char* ssid = "your-ssid";   // WiFi network name (SSID)
const char* password = "your-password";   // WiFi password

void setup() {
    Serial.begin(115200);
    delay(10);

    Serial.println("\n\nConnecting to WiFi...");
    Serial.printf("SSID: %s\n", ssid);

    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
    // Empty loop for now
}

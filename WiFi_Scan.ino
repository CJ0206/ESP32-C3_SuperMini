#include "WiFi.h"

void setup() {
    Serial.begin(115200);
    
    // Configure WiFi to station mode and disconnect from previous connections
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    delay(100); // Short delay before proceeding
    Serial.println("Setup complete");
}

void loop() {
    Serial.println("Starting WiFi scan...");

    // Scan for available networks
    int numNetworks = WiFi.scanNetworks();
    Serial.println("Scan complete");

    if (numNetworks == 0) {
        Serial.println("No networks found");
    } else {
        Serial.printf("%d networks found\n", numNetworks);

        for (int i = 0; i < numNetworks; i++) {
            // Print SSID, signal strength, and encryption type
            Serial.printf("%d: %s (%d dBm) %s\n", 
                          i + 1, 
                          WiFi.SSID(i).c_str(), 
                          WiFi.RSSI(i), 
                          (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "*Secured");
        }
    }
    
    Serial.println(); // Add a blank line for better readability

    // Wait before scanning again
    delay(5000);
}

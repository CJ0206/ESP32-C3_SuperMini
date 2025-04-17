#include "WiFi.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "time.h"

// WiFi Credentials
const char* ssid = "your-ssid";   // WiFi network name (SSID)
const char* password = "your-password";   // WiFi password

// Location-Based Timezone (Change to your region)
const char* timeZone = "Europe/London"; 

// NTP Server
const char* ntpServer = "pool.ntp.org";

// OLED Display Setup
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

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

    // Initialize OLED Display
    u8g2.begin();
    u8g2.setFont(u8g2_font_helvB12_tf); // Larger font

    // Set timezone automatically based on region
    setenv("TZ", timeZone, 1);
    tzset();

    // Initialize NTP time
    configTime(0, 0, ntpServer);
}

void loop() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }

    // Format Time & Date
    char timeString[10];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);

    char dateString[6];
    strftime(dateString, sizeof(dateString), "%d/%m", &timeinfo);

    // Display on OLED (Bigger text)
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB12_tf); // Bigger font

    u8g2.setCursor(10, 30);   // Time (left)
    u8g2.print(timeString);

    u8g2.setCursor(90, 30);   // Date (right)
    u8g2.print(dateString);

    u8g2.sendBuffer();

    delay(1000); // Refresh every second
}

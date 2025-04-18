#include "WiFi.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "time.h"

// WiFi Credentials
const char* ssid = "your-ssid";         // WiFi network name (SSID)
const char* password = "your-password"; // WiFi password

// Location-Based Timezone (Change to your region)
const char* timeZone = "Europe/London";

// NTP Server
const char* ntpServer = "pool.ntp.org";

// OLED Display Setup (128×64 OLED using software I2C)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE
);

// Draw a WiFi icon as a series of four bars.
void drawWiFiIcon(int x, int y, int activeBars) {
  const int barWidth = 3;
  const int barGap = 2;
  // Progressive bar heights for a retro look.
  const int barHeights[4] = {3, 5, 7, 9};
  int baseLine = y + 10;  // This aligns all bars along the bottom.
  
  for (int i = 0; i < 4; i++) {
    int barX = x + i * (barWidth + barGap);
    int barHeight = barHeights[i];
    int barY = baseLine - barHeight;
    u8g2.drawFrame(barX, barY, barWidth, barHeight);
    if (i < activeBars) {
      if ((barWidth - 2) > 0 && (barHeight - 2) > 0)
        u8g2.drawBox(barX + 1, barY + 1, barWidth - 2, barHeight - 2);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("\n\nConnecting to WiFi...");
  Serial.printf("SSID: %s\n", ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());

  // Initialize the OLED display.
  u8g2.begin();
  u8g2.setFont(u8g2_font_helvR14_tf);

  // Set timezone and configure NTP.
  setenv("TZ", timeZone, 1);
  tzset();
  configTime(0, 0, ntpServer);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // Format the time as "HH:MM", you can add :%S to add ":SS"
  char timeString[9];  // "HH:MM" plus null terminator.
  strftime(timeString, sizeof(timeString), "%H:%M", &timeinfo);
  
  // Format the date as "DD/MM"
  char dateString[6];  // "DD/MM" plus null terminator.
  strftime(dateString, sizeof(dateString), "%d/%m", &timeinfo);

  // Map WiFi RSSI to active bars (0 to 4).
  int rssi = WiFi.RSSI();
  int bars = 0;
  if (rssi >= -50)       bars = 4;
  else if (rssi >= -60)  bars = 3;
  else if (rssi >= -70)  bars = 2;
  else if (rssi >= -80)  bars = 1;
  else                   bars = 0;
  
  u8g2.clearBuffer();
  
  // Draw the WiFi icon in the top left.
  drawWiFiIcon(2, 2, bars);

  // Draw the date in the top right.
  u8g2.setFont(u8g2_font_freedoomr10_mu);
  u8g2.setFontPosTop();
  // Expand the reference height so the lower part of the font is visible.
  u8g2.setFontRefHeightAll();
  int dateWidth = u8g2.getStrWidth(dateString);
  // Place the date at y = 4 (adjust if needed).
  u8g2.setCursor(128 - dateWidth - 2, 4);
  u8g2.print(dateString);

  // Draw the time in the center.
  u8g2.setFont(u8g2_font_freedoomr25_tn);
  u8g2.setFontPosTop();
  int timeWidth = u8g2.getStrWidth(timeString);
  int timeX = (128 - timeWidth) / 2;
  u8g2.setCursor(timeX, 28);
  u8g2.print(timeString);

  u8g2.sendBuffer();
  delay(1000);
}

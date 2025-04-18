#include "WiFi.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "time.h"

// WiFi Credentials
const char* ssid = "your-ssid";         // WiFi network name (SSID)
const char* password = "your-password"; // WiFi password

// Daylight Savings-Aware Timezone
const char* timeZone = "GMT0BST,M3.5.0/1,M10.5.0/2";

// NTP Servers
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const char* ntpServer3 = "ntp2d.mcc.ac.uk";

// OLED Display Setup (128×64 OLED using software I2C)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE
);

void drawWiFiIcon(int x, int y, int activeBars) {
  const int barWidth = 3;
  const int barGap = 2;
  const int barHeights[4] = {3, 5, 7, 9};
  int baseLine = y + 10;
  
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
  configTime(0, 0, ntpServer1, ntpServer2, ntpServer3);

  Serial.println("Checking DST settings...");
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.printf("Raw time: %02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min);
    Serial.printf("Daylight Saving (tm_isdst): %d\n", timeinfo.tm_isdst);
  } else {
    Serial.println("Failed to get time");
  }
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // Adjust for DST manually if necessary
  if (timeinfo.tm_isdst == 0) {
    timeinfo.tm_hour += 1;
  }

  // Format the time as "HH:MM"
  char timeString[9];
  strftime(timeString, sizeof(timeString), "%H:%M", &timeinfo);
  
  // Format the date as "DD/MM"
  char dateString[6];
  strftime(dateString, sizeof(dateString), "%d/%m", &timeinfo);

  // Map WiFi RSSI to active bars (0 to 4)
  int rssi = WiFi.RSSI();
  int bars = (rssi >= -50) ? 4 : (rssi >= -60) ? 3 : (rssi >= -70) ? 2 : (rssi >= -80) ? 1 : 0;
  
  u8g2.clearBuffer();
  
  // Draw the WiFi icon in the top left
  drawWiFiIcon(2, 2, bars);

  // Draw the date in the top right
  u8g2.setFont(u8g2_font_freedoomr10_mu);
  u8g2.setFontPosTop();
  u8g2.setFontRefHeightAll();
  int dateWidth = u8g2.getStrWidth(dateString);
  u8g2.setCursor(128 - dateWidth - 2, 4);
  u8g2.print(dateString);

  // Draw the time in the center
  u8g2.setFont(u8g2_font_freedoomr25_tn);
  u8g2.setFontPosTop();
  int timeWidth = u8g2.getStrWidth(timeString);
  int timeX = (128 - timeWidth) / 2;
  u8g2.setCursor(timeX, 28);
  u8g2.print(timeString);

  u8g2.sendBuffer();
  delay(1000);
}

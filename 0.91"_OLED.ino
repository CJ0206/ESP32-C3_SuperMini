#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Initialize the display using software I2C
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
    u8g2.begin();
}

void loop(void) {
    u8g2.clearBuffer();                        // Clear the internal memory
    u8g2.setFont(u8g2_font_helvB12_tr);        // Use Helvetica Bold 12px

    // Draw text on the screen with improved spacing
    u8g2.drawStr(0, 20, "Hello World!");
    u8g2.drawStr(0, 40, "OLED Display");
    u8g2.drawStr(0, 60, "Font Test");

    u8g2.sendBuffer();                         // Transfer internal memory to the display
    delay(1000);                               // Delay before refreshing
}

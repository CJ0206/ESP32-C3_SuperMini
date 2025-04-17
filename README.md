# ESP32-C3 SuperMini <a href='https://ko-fi.com/christianjameswatkins' target='_blank'><img height='35' align='right' style='border:0px;height:46px;' src='https://storage.ko-fi.com/cdn/kofi1.png?v1' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

Below is a collection of random projects for the [ESP32-C3 SuperMini](https://www.aliexpress.com/item/1005007446928015.html).

To get started:
1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software/).
2. Start the Arduino IDE.
3. Add the ESP32 board package to the Arduino IDE:
   - File > Preferences > Addional Boards Manage URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools > Board > Board Manager > search for `esp32` > Install the one by `Espressif Systems`
   - Tools > Development Board > select `ESP32C3 Dev Module`
   - Tools > Ports > select the relevant COM port, mine endedwith `(ESP32 Family Device)`
  
If you are having issues with the COM port not being recognised by the IDE try holding the `BOOT` button on the board, if that does not work try holding the `BOOT` and `RESET` buttons, release the `RESET` button, then release the `BOOT` button and the poard should enter download mode. Each connectionm needs to re-enter the download mode.

If the program does not run after uploading, try pressing the `RESET` button to execute the upload.

If you are having issues withh the serial port not printing to terminal ensure `USB CDC On Boot` is `Enabled` in the tools menu.

## [Flashing LED](LED_Flash.ino)

This code will flash the boards onboard LED once every second.

## [WiFi Scan](WiFi_Scan.ino)

This code scans for available WiFi networks every 5 seconds and prints their details to the serial monitor. 

Go to Tools > Serial Monitor to view the output (or use the shortcut `Ctrl`+`Shift`+`M`)

You should see an output similar to the below:
```
Starting WiFi scan...
Scan complete
5 networks found
1: NETWORK1 (-46 dBm) *Secured
2: NETWORK2 (-56 dBm) *Secured
3: NETWORK3 (-73 dBm) Open
```

## [Connecting to Wifi](WiFi_Connect.ino)

This code will connect the board to a WiFi network using a predefined SSID and password. It starts by initializing serial communication, then attempts to connect to the specified WiFi network, displaying connection progress in the serial monitor. Once connected, it prints the device's assigned IP address. There is an empty loop at the end to run your own code.

Go to Tools > Serial Monitor to view the output (or use the shortcut `Ctrl`+`Shift`+`M`)

You should see an output similar to the below:
```
Connecting to WiFi...
.
WiFi connected!
IP Address: 192.168.1.22
```

## [Using a 0.91" OLED](0.91"_OLED.ino)

To use a [0.91" I2C OLED](https://www.aliexpress.com/item/1005006365845676.html) with the [ESP32-C3 SuperMini](https://www.aliexpress.com/item/1005007446928015.html) you will need to wire the devices togerther:

| ESP32SuperMini | 0.91" OLED |
| -------------- | ---------- |
| 5V | VCC |
| GND | GND |
| SDA (GPIO 8) | SDA |
| SCL (GPIO 9) | SCL |

We'll also need to includee the drivers for the OLED by going to:
Sketch > Include Library > Manage Libraries...

Search for `u8g2` and install the [library](https://github.com/olikraus/u8g2) by `oliver`

## [RTC](RTC.ino)

This code will display a simple real time clock (HH:MM:SS) and date (DD/MM) on a 0.91" I2C OLED.

/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
Funktion: ESP32 TFT 1.77" und 1.8" Font-Test 
**************************************************************************************************
Version: 19.01.2023
**************************************************************************************************
Board: ESP32vn IoT UNO V1.0.4
**************************************************************************************************
Libraries:
https://github.com/espressif/arduino-esp32/tree/master/libraries
C:\Users\User\Documents\Arduino
D:\gittemp\Arduino II\A156_Wetterdaten_V3
**************************************************************************************************
C++ Arduino IDE V1.8.19
**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
http://arduino.esp8266.com/stable/package_esp8266com_index.json
**************************************************************************************************
*Verdrahtung:
*
ESP32 1,8" TFT
5V    VCC
GND   GND
G12   CS
G14   RESET
G13    A0
G23   SDA
G18   SCK
3.3V  LED

ESP   1.77" TFT
GND    GND
5V   VCC
G18   SCK
G23   SDA
G13   RES
G12   RS
3.3V  CS

**************************************************************************************************
Die User_Setup.h in C:\Users\User\Documents\Arduino\libraries\TFT_eSPI\ kopieren!!!!!

User_Setup.h Library fuer TFT 1.77" und 1,8" TFT 
// TFT 1.77" Library

#define ST7735_DRIVER
#define TFT_WIDTH 128
#define TFT_HEIGHT 160
#define ST7735_REDTAB
#define TFT_BACKLIGHT_ON HIGH // HIGH or LOW are options

// ###### EDIT THE PIN NUMBERS IN THE LINES FOLLOWING TO SUIT YOUR ESP32 SETUP ######
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 12 // Chip select control pin
#define TFT_DC 13 // Data Command control pin
#define TFT_RST 14 // Reset pin (could connect to RST pin)

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
#define SMOOTH_FONT
#define SPI_FREQUENCY  27000000 // Actually sets it to 26.67MHz = 80/3

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  20000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY  2500000

// Transactions are automatically enabled by the library for an ESP32 (to use HAL mutex)
// so changing it here has no effect

// #define SUPPORT_TRANSACTIONS


**************************************************************************************************/

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 3000

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

float t = 23;
float f = 43;
float w = 2.3;

float t2 = 21;
float f2 = 53;
float w2 = 1.2;


void setup(void) {
  tft.init();
  tft.setRotation(1);
}

void loop() {

  tft.setTextSize (2);

//---------- Seite 1 ----------------------------
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
  tft.drawString("Wetterdaten", 12, 0);
  tft.drawString("Tegernsee", 22, 26);

  
  tft.drawString("Temp :", 0, 52);
  tft.setCursor (76,52);
  tft.print (t,1);tft.print ("\xF7""C");

  
  tft.drawString("Luftf: ", 0, 78);
    tft.setCursor (76,78);
  tft.print (f,1);tft.print ("%");

  
  tft.drawString("Wind :", 0, 104); 
    tft.setCursor (76,104);
  tft.print (w,1);tft.print ("m/s");

  
 delay(WAIT);

 

//---------- Seite 2 ----------------------------
    tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
  tft.drawString("Wetterdaten", 12, 0);
  tft.drawString("Schliersee", 22, 26);

  
  tft.drawString("Temp :", 0, 52);
  tft.setCursor (76,52);
  tft.print (t2,1);tft.print ("\xF7""C");

  
  tft.drawString("Luftf: ", 0, 78);
    tft.setCursor (76,78);
  tft.print (f2,1);tft.print ("%");

  
  tft.drawString("Wind :", 0, 104); 
    tft.setCursor (76,104);
  tft.print (w2,1);tft.print ("m/s");

  
 delay(WAIT);

}

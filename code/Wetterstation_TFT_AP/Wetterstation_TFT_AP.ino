/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
  Funktion: ESP32 Wetterstation mit WLAN-Manager - Wetterdaten Tegernsee und Raumklima 1.77" TFT Display DHT11 
  optional mit OLED1,3" Display
**************************************************************************************************
  Version: 21.01.2023
**************************************************************************************************
  Board: ESP32vn IoT UNO V1.0.4 /1.0.6

**************************************************************************************************
  C++ Arduino IDE V1.8.19
**************************************************************************************************
  Einstellungen:
  https://dl.espressif.com/dl/package_esp32_index.json
  http://dan.drown.org/stm32duino/package_STM32duino_index.json
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
**************************************************************************************************

  Server-Abfrage:
  http://api.openweathermap.org/data/2.5/weather?q=Tegernsee,DE&APPID=c2c58a80ce6377889800c284c540c505

  JSON-Meldung:
  {"coord":{"lon":11.7527,"lat":47.7097},
  "weather":
  [{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}],

  "base":"stations",
  "main":
  {"temp":288.35,
  "feels_like":288.54,
  "temp_min":285.19,
  "temp_max":291.7,
  "pressure":1016,
  "humidity":100,
  "sea_level":1016,
  "grnd_level":932},
  "visibility":273,
  "wind":{"speed":1.75,"deg":30,"gust":3.01},

  "clouds":{"all":100},
  "dt":1625628595,
  "sys":{"type":2,"id":2001431,"country":"DE","sunrise":1625628176,"sunset":1625685150},
  "timezone":7200,"id":2823679,"name":"Tegernsee","cod":200



  Beispiele für Schriftarten:
  Schrift­hö­he in Pixeln (px)
  6px: u8g2_font_5x7_tr
  7px: u8g2_font_torussansbold8_8r
  8px: u8g2_font_ncenB08_tr
  10px: u8g2_font_t0_15b_me
  12px: u8g2_font_helvB12_tf
  13px: u8g2_font_t0_22_te
  14px: u8g2_font_helvB14_tf
  17px: u8g2_font_timB18_tf
  18px: u8g2_font_lubB18_tr
  20px: u8g2_font_courB24_tf
  23px: u8g2_font_timB24_tf
  25px: u8g2_font_helvR24_tf
  32px: u8g2_font_logisoso32_tf
  42px: u8g2_font_fub42_tf
  58px: u8g2_font_logisoso58_tf
  62px: u8g2_font_logisoso62_tn



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
GND   GND
5V    VCC
G18   SCK
G23   SDA (MOSI)
G14   RES (RST)
G13   RS (DC)
G12   CS
3.3V  LEDA

**************************************************************************************************
*TFT-Display:
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

#include <SimpleDHT.h> //by Winlin 1.0.14       
int pinDHT11 = 27;
SimpleDHT11 dht11(pinDHT11);
#include <Adafruit_Sensor.h> //by Adafruit 0.8.0
#include <WiFi.h> //by Arduino 1.2.7
#include <WiFiUdp.h> 
#include <NTPClient.h> //Easy NTPC ba Harshen Alva 1.1.0

#include <HTTPClient.h> //Arduino HTTPClient by Arduino 0.4.0
#include <Arduino_JSON.h> //by Arduino 0.1.0


String openWeatherMapApiKey = "c2c58a80ce6377889800c284c540c505";
String city = "Tegernsee";
String countryCode = "DE";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000; //10 Sekunden
String jsonBuffer;

//-----------------------------------------------
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
unsigned long delayTime;
//-----------------------------------------------
int ktemp;
int mintemp;
int maxtemp;
int sicht;

//----------------clock----------------------------
// Define NTP Client to get time

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "de.pool.ntp.org");

//Week Days
String weekDays[7] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

//Month names
String months[12] = {"Jan.", "Feb.", "März", "April", "Mai", "Juni", "Juli", "Aug.", "Sept.", "Okt.", "Nov.", "Dez."};
//-----------------------------------------------


//******** TFT ************************************
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
//*****************************************************


void setup() {
  Serial.begin(115200);
   //init WiFi
  Serial.println("Connecting to WiFi");
  while (!makeWLAN()) {
    Serial.println("Cannot connect :(");
    delay(1000);
  }

  Serial.println("Aktualisierung ist auf 10 Sekunden eingestellt.");

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600 Winterzeit
  // GMT +2 = 7200 Sommerzeit
  timeClient.setTimeOffset(3600);

  u8g2.begin();

  int BildschirmBreite = u8g2.getDisplayWidth();
  int BildschirmHoehe = u8g2.getDisplayHeight();

  //Smiley XBM erstellt mit GIMP
#define smiley_width 64
#define smiley_height 64
  static unsigned char smiley[] = {
    0x00, 0x00, 0x00, 0xd0, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff,
    0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x07, 0x00, 0x00,
    0x00, 0x00, 0xfc, 0x5f, 0xfd, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
    0x80, 0x7f, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0xfc, 0x01, 0x00,
    0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xf0, 0x01, 0x00,
    0x00, 0xc0, 0x0f, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00,
    0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x1e, 0x00, 0x00,
    0x00, 0x00, 0x7c, 0x00, 0x00, 0x1f, 0x00, 0x03, 0x80, 0x01, 0xf8, 0x00,
    0x80, 0x07, 0x80, 0x03, 0xc0, 0x03, 0xf0, 0x00, 0xc0, 0x07, 0x80, 0x07,
    0xc0, 0x03, 0xe0, 0x01, 0xc0, 0x03, 0xc0, 0x07, 0xc0, 0x07, 0xc0, 0x03,
    0xe0, 0x01, 0xc0, 0x07, 0xe0, 0x07, 0x80, 0x07, 0xe0, 0x00, 0xc0, 0x0f,
    0xe0, 0x07, 0x80, 0x07, 0xf0, 0x00, 0xc0, 0x0f, 0xe0, 0x07, 0x00, 0x0f,
    0x70, 0x00, 0xc0, 0x0f, 0xe0, 0x07, 0x00, 0x0e, 0x78, 0x00, 0xc0, 0x0f,
    0xe0, 0x07, 0x00, 0x1e, 0x38, 0x00, 0xe0, 0x0f, 0xe0, 0x07, 0x00, 0x1e,
    0x3c, 0x00, 0xc0, 0x0f, 0xe0, 0x07, 0x00, 0x1c, 0x1c, 0x00, 0xc0, 0x07,
    0xe0, 0x07, 0x00, 0x3c, 0x1c, 0x00, 0xc0, 0x07, 0xe0, 0x07, 0x00, 0x38,
    0x1c, 0x00, 0xc0, 0x07, 0xc0, 0x03, 0x00, 0x38, 0x1e, 0x00, 0x80, 0x07,
    0xc0, 0x03, 0x00, 0x38, 0x1e, 0x00, 0x80, 0x03, 0x80, 0x01, 0x00, 0x38,
    0x0e, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x78, 0x0e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78,
    0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x78, 0x0e, 0x7c, 0x00, 0x00,
    0x00, 0x00, 0x3e, 0x70, 0x0e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38,
    0x1e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x18, 0x78, 0x1e, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x38, 0x1e, 0x30, 0x00, 0x00, 0x00, 0x00, 0x08, 0x38,
    0x1c, 0x20, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x38, 0x1c, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x38, 0x1c, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x3c,
    0x3c, 0x40, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1c, 0x38, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x1c, 0x78, 0x80, 0x01, 0x00, 0x00, 0x00, 0x01, 0x1e,
    0x78, 0x00, 0x01, 0x00, 0x00, 0x80, 0x01, 0x0e, 0xf0, 0x00, 0x07, 0x00,
    0x00, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0x04, 0x00, 0x00, 0x60, 0x00, 0x07,
    0xe0, 0x01, 0x1c, 0x00, 0x00, 0x38, 0x80, 0x07, 0xc0, 0x03, 0x70, 0x00,
    0x00, 0x0c, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x01, 0x00, 0x07, 0xe0, 0x03,
    0x80, 0x07, 0x80, 0x17, 0xf0, 0x01, 0xe0, 0x01, 0x00, 0x0f, 0x00, 0xfc,
    0x3f, 0x00, 0xf0, 0x00, 0x00, 0x1f, 0x00, 0x40, 0x01, 0x00, 0x78, 0x00,
    0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0xfc, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x80, 0x0f, 0x00,
    0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xc0, 0x1f, 0x00,
    0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0xfe, 0x2f, 0xf4, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff,
    0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
/*
  u8g2.setFont(u8g2_font_courR10_tf);
  u8g2.firstPage();

  do {
    u8g2.clearBuffer();
    u8g2.drawXBM(32, 1, smiley_width, smiley_height, smiley);
  } while ( u8g2.nextPage() );
  delay(3000);
*/

  //************* TFT ****************************
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.drawXBitmap(50, 30, smiley, 64, 64, 0xFFFF);
  tft.setTextSize (2);
        tft.setTextColor(TFT_BLUE, TFT_BLACK); 
  tft.drawString("Ich lade ", 30, 5);
  tft.drawString("die Daten", 30, 106);
    delay(1000);
//***********************************************
;
}

void loop() {


  //----------------Clock-----------------------------------------
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute);

  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);

  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon + 1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  String currentMonthName = months[currentMonth - 1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  int currentYear = ptm->tm_year + 1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  Serial.println("");

  //---------------------------------------------------------

  Serial.println("=================================");
  Serial.println("Abfrage DHT11 Raumklima:");
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(2000);
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((float)temperature);
  Serial.print(" *C, ");
  Serial.print((float)humidity);
  Serial.println(" RH%");
  delay(500); // DHT11 Abfragezeit

  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi Status
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Eingabe fehlgeschlagen!");
        return;
      }
      ktemp = (myObject["main"]["temp"]);
      mintemp = (myObject["main"]["temp_min"]);
      maxtemp = (myObject["main"]["temp_max"]);

      //      Serial.print("JSON object = ");
      Serial.println("Meine Wetterdaten:");
      Serial.print("Ort: ");
      Serial.println(myObject["name"]);
      Serial.print("Aktuelle Temperatur: ");
      Serial.print(myObject["main"]["temp"]);
      Serial.println("K ");
      Serial.print("Aktuelle Temperatur: ");
      Serial.print(ktemp - 273.15);
      Serial.println("°C ");
      Serial.print("Tages-Min. Temperatur: ");
      Serial.print(mintemp);
      Serial.println("K ");

      Serial.print("Tages-Min. Temperatur: ");
      Serial.print(mintemp - 273.15);
      Serial.println("°C ");

      Serial.print("Tages-Max. Temperatur: ");
      Serial.print(maxtemp);
      Serial.println("K ");

      Serial.print("Tages-Max. Temperatur: ");
      Serial.print(maxtemp - 273.15);
      Serial.println("°C ");

      Serial.print("Wolken: ");
      Serial.print(myObject["clouds"]["all"]);
      Serial.println("% ");

      Serial.print("Luftdruck: ");
      Serial.print(myObject["main"]["pressure"]);
      Serial.println("hPa");
      Serial.print("Luftfeuchte: ");
      Serial.print(myObject["main"]["humidity"]);
      Serial.println("% ");
      Serial.print("Wind Geschwindigkeit: ");
      Serial.print(myObject["wind"]["speed"]);
      Serial.println("m/s ");

      sicht = (myObject["visibility"]);
      sicht = sicht / 100;
      Serial.print("Sicht: ");
      Serial.print(sicht);
      Serial.println("km ");

      //---------------------------------------------------------


//********* TFT *****************
  tft.setTextSize (2);

  //---------- Seite 1 ----------------------------
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
  tft.drawString("Datum-Uhrzeit", 5, 0);
//  tft.drawString("Tegernsee", 22, 26);

  
 // tft.drawString("Temp :", 0, 52);
  tft.setCursor (42,26);
  tft.print (weekDay);
    tft.setCursor (0,52);
    tft.print(monthDay); tft.print(". "); tft.print (currentMonthName); tft.print(" "); tft.print (currentYear);
;

  
      tft.setCursor (20,88);
      tft.setTextColor(0xFBE0, TFT_BLACK); 
        tft.setTextSize (4);
          tft.print(currentHour);
        tft.print(":");
         if (currentMinute < 10) {
           tft.print("0");
           tft.print(currentMinute);
         }else{
            tft.print(currentMinute);
            }
  tft.setTextSize (2);
 /* 
  tft.drawString("Wind :", 0, 104); 
    tft.setCursor (76,104);
  tft.print (w,1);tft.print ("m/s");
*/
  
      delay(4000);
 

//---------- Seite 2 ----------------------------
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
  tft.drawString("Raumklima", 22, 26);
//  tft.drawString("Tegernsee", 22, 26);

  
  tft.drawString("Temp :", 0, 52);
  tft.setCursor (76,52);
  tft.print (temperature);tft.print ("\xF7""C");

  
  tft.drawString("Luftf: ", 0, 78);
    tft.setCursor (76,78);
  tft.print (humidity);tft.print ("%");

 /* 
  tft.drawString("Wind :", 0, 104); 
    tft.setCursor (76,104);
  tft.print (w,1);tft.print ("m/s");
  */

            
      delay(4000);

 

//---------- Seite 3 ----------------------------
    tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
  tft.drawString("Wetterdaten", 12, 0);
  tft.drawString("Tegernsee", 22, 26);

  
  tft.drawString("Temp: ", 0, 52);
  tft.setCursor (65,52);
  tft.print (maxtemp - 273.15,1);tft.print ("\xF7""C");

   
  tft.drawString("LuDr: ", 0, 78);
    tft.setCursor (65,78);
  tft.print (myObject["main"]["pressure"]);tft.print ("hPa");
  
  tft.drawString("Wind: ", 0, 104); 
    tft.setCursor (65,104);
  tft.print (myObject["wind"]["speed"]);tft.print ("m/s");



 

  
//      delay(3000);;

 //*******************************************

/*
      Serial.println("=================================");

      u8g2.setFont(u8g2_font_courR10_tf);
      u8g2.firstPage();


      do {
        u8g2.clearBuffer();
        u8g2.setCursor(6, 10);
        u8g2.print("Datum-Uhrzeit");
        u8g2.setFont(u8g2_font_courR10_tf);
        u8g2.setCursor(30, 27);
        u8g2.print(weekDay);
        u8g2.setCursor(3, 43);
        u8g2.print(monthDay);
        u8g2.print(". ");
        u8g2.print(currentMonthName);
        u8g2.print(" ");
        u8g2.print(currentYear);
        u8g2.setFont(u8g2_font_courB14_tf);
        u8g2.setCursor(40, 63);

        u8g2.print(currentHour);
        u8g2.print(":");
         if (currentMinute < 10) {
           u8g2.print("0");
           u8g2.print(currentMinute);
         }else{
            u8g2.print(currentMinute);
            }
  /*      
       u8g2.print(currentMinute);
        u8g2.print(":");


         if (currentSecond) < 10) {
           u8g2.print("0");
           u8g2.print(currentSecond);
         }else{
            u8g2.print(currentSecond);
          }
*/
/*
        
//        u8g2.print(currentSecond);
        u8g2.setFont(u8g2_font_courR10_tf);
      } while ( u8g2.nextPage() );
      delay(2000);


      do {
        u8g2.clearBuffer();
        u8g2.setCursor(12, 10);
        u8g2.print("Raumklima");

        u8g2.setCursor(2, 35);
        u8g2.print("Temp: ");
        u8g2.setCursor(51, 35);
        u8g2.print(temperature);
        u8g2.print("\xB0""C");

        u8g2.setCursor(2, 51);
        u8g2.print("LuFe:");
        u8g2.setCursor(51, 51);
        u8g2.print(humidity);
        u8g2.print("%");

      } while ( u8g2.nextPage() );
      delay(2000);


      do {
        u8g2.clearBuffer();
        u8g2.setCursor(12, 10);
        u8g2.print("Wetterdaten");
        u8g2.setCursor(18, 22);
        u8g2.print("Tegernsee");

        u8g2.setCursor(2, 35);
        u8g2.print("Temp: ");
        u8g2.setCursor(51, 35);
        u8g2.print(maxtemp - 273.15);
        u8g2.print("\xB0""C");

        u8g2.setCursor(2, 49);
        u8g2.print("Wind:");
        u8g2.setCursor(51, 49);
        u8g2.print(myObject["wind"]["speed"]);
        u8g2.print("m/s");
        /*
              u8g2.setCursor(2, 48);
              u8g2.print("LuFe:");
              u8g2.setCursor(51, 48);
              u8g2.print(myObject["main"]["humidity"]);
              u8g2.print("%");
        */
        /*
        u8g2.setCursor(2, 63);
        u8g2.print("LuDr:");
        u8g2.setCursor(51, 63);
        u8g2.print(myObject["main"]["pressure"]);
        u8g2.print("hPa");

      } while ( u8g2.nextPage() );
      delay(1000);
*/
      //---------------------------------------------------------
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Domain-Name
  http.begin(client, serverName);

  // HTTP POST request 200=OK
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP-Antwort Code (200=OK): ");
    Serial.println(httpResponseCode);
    Serial.println("Server-Meldung: ");
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  return payload;
}

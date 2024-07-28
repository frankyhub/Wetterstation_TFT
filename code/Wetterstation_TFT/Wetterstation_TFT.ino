/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
  Funktion: ESP32 Wetterstation mit WLAN-Manager - Wetterdaten Tegernsee und Raumklima DHT11 TFT 1.77"
            Gas-Sensor MQ135, Buzzer und Gas LED-Ampel
            Automatische So/Wi-Zeit Umstelung


  OpenWeather API 31.10.2022: Wetterstation_AP    b2bc7fc61c4cc3545e6bbf151b58e1dd

**************************************************************************************************
  Version: 28.07.2024
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

  Server-Abfrage Programm:
  http://api.openweathermap.org/data/2.5/weather?q=Tegernsee,DE&APPID=c2c58a80ce6377889800c284c540c505

  Server-Abfrage 2. API:
  http://api.openweathermap.org/data/2.5/weather?q=Tegernsee,DE&APPID=b2bc7fc61c4cc3545e6bbf151b58e1dd

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


  *********************************************  MQ-135 Luftqualität-Sensor  ******************************************************
  Beschreibung des MQ-135 Luftqualität-Sensor

  Sensor zum Messen der Konzentration von verschiedenen Giftgasen wie Benzon, Alkohol, Rauch sowie
  Verunreinigungen in der Luft. Der MQ-135 misst eine Gaskonzentration von 10 bis 1000ppm und ist ideal zum Erkennen eines
  Gaslecks, als Gas-Alarm oder für sonstige Robotik- und Microcontroller-Projekte.

  Die Sensoren der MQ-Serie benutzen ein kleines Heizelement mit einem elektronisch-chemischen Sensor. Sie sind empfindlich
  gegenüber verschiedenster Gase und eignen sich zur Verwendung in Räumen.

  Der Sensor hat eine hohe Empfindlichkeit und schnelle Reaktionszeit, benötigt allerdings einige Minuten bis er genaue Messwerte
  ausgibt, da der Sensor sich erst aufheizen muss. Die Messwerte des Sensors werden als analoger Wert ausgegeben, welcher mit einem
  Microcontroller einfach ausgewertet werden kann.

  Pinbelegung:

  VCC - Stromversorgung 5V
  GND - Masseanschluss
  AOUT - Analoger Output
  DOUT -  Digitaler Output
  Zum Einstellen des Schwellenwerts besitzt das Modul ein Potentiometer. Am digitalen Pin gibt der Sensor lediglich eine 0 oder 1 aus,
  je nach dem ob der eingestellte Schwellenwert überschritten wird.

  Vorsicht: Der Sensor wird im Betrieb warm!

  Details:
  Auflösung: 10~1000ppm
  Sensitive Widerstand: 2KΩ to 20KΩ in 100ppm CO
  Genauigkeit: ≥ 3%
  Reaktionszeit: ≤ 1s
  Ansprechzeit nach Einschalten: ≤ 30s
  Heizstrom: ≤ 180mA
  Heizspannung: 5.0V±0.2V / 1.5±0.1V
  Eingangsspannung: 5V
  Heizenergieleistung ca.: 350mW
  Abmessungen: ca. 30mm (Länge) * 20mm (Breite)


  !!!!!!!!!!!!!!!!! /TFT_eSPI/User_Setup.h !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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


#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
unsigned long delayTime;

//******** TFT ************************************
// New background colour
#define TFT_BROWN 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 2000

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();  // Aktualisiere User_Setup.h

unsigned long targetTime = 0; // Used for testing draw times

//******** DHT ************************************
#include <SimpleDHT.h> //by Winlin 1.0.14       
int pinDHT11 = 27;
SimpleDHT11 dht11(pinDHT11);
#include <Adafruit_Sensor.h> //by Adafruit 0.8.0
#include <WiFi.h> //by Arduino 1.2.7
#include <WiFiUdp.h>
#include <NTPClient.h> //Easy NTPC ba Harshen Alva 1.1.0

//*************************************************

#include <HTTPClient.h> //Arduino HTTPClient by Arduino 0.4.0
#include <Arduino_JSON.h> //by Arduino 0.1.0

//*************************************************
//------- Gas Sensor MQ135 ------------
int Buzzer = 26;
int Gas_analog = 39;
//int Gas_digital = 2;

#define led_rot 25
#define led_gelb 17
#define led_gruen 16
//----------------------------------------


//*************************************************
String openWeatherMapApiKey = "b2bc7fc61c4cc3545e6bbf151b58e1dd"; //Workshop API


String city = "Tegernsee";
String countryCode = "DE";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000; //120 Sekunden
String jsonBuffer;
//*************************************************


//-----------------------------------------------
int ktemp;
int mintemp;
int maxtemp;
int sicht;

//----------------clock----------------------------
// Internetzeit
#define MY_NTP_SERVER "de.pool.ntp.org"
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
//#include <time.h> // time() ctime()
time_t now;    // this is the epoch
tm tm;         // the structure tm holds time information in a more convenient way
void showTime() {
  time(&now); // read the current time
  localtime_r(&now, &tm); // update the structure tm with the current time
  Serial.print("year:");
  Serial.print(tm.tm_year + 1900); // years since 1900
  Serial.print("\tmonth:");
  Serial.print(tm.tm_mon + 1); // January = 0 (!)
  Serial.print("\tday:");
  Serial.print(tm.tm_mday); // day of month
  Serial.print("\thour:");
  Serial.print(tm.tm_hour); // hours since midnight 0-23
  Serial.print("\tmin:");
  Serial.print(tm.tm_min); // minutes after the hour 0-59
  Serial.print("\tsec:");
  Serial.print(tm.tm_sec); // seconds after the minute 0-61*
  Serial.print("\twday");
  Serial.print(tm.tm_wday); // days since Sunday 0-6
  if (tm.tm_isdst == 1) // Daylight Saving Time flag
    Serial.print("\tDST");
  else
    Serial.print("\tstandard");
  Serial.println();
}



// Define NTP Client to get time

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "de.pool.ntp.org");

//Week Days
String weekDays[7] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

//Month names
String months[12] = {"Jan.", "Feb.", "M""\xE4""rz", "April", "Mai", "Juni", "Juli", "Aug.", "Sept.", "Okt.", "Nov.", "Dez."};
//-----------------------------------------------



void setup() {

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
//***********************************************;


  Serial.begin(115200);
  //init WiFi
  Serial.println("Connecting to WiFi");
  while (!makeWLAN()) {
    Serial.println("Cannot connect :(");
    delay(1000);
  }
  //******** ESP32 Wi-So Zeit ***************
  Serial.println("\nNTP TZ DST - bare minimum");

#ifdef ARDUINO_ARCH_ESP32
  // ESP32 seems to be a little more complex:
  configTime(0, 0, MY_NTP_SERVER); // 0, 0 because we will use TZ in the next line
  setenv("TZ", MY_TZ, 1); // Set environment variable with your time zone
  tzset();
#else
  // ESP8266
  configTime(MY_TZ, MY_NTP_SERVER); // --> for the ESP8266 only
#endif



  //------- Gas Sensor MQ135 ------------
  pinMode(Buzzer, OUTPUT);
  //    pinMode(Gas_digital, INPUT);

  pinMode(led_rot, OUTPUT);
  pinMode(led_gruen, OUTPUT);
  pinMode(led_gelb, OUTPUT);
  //----------------------------------------

  Serial.println("Aktualisierung ist auf 10 Sekunden eingestellt.");

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600 Winterzeit
  // GMT +2 = 7200 Sommerzeit
  //  timeClient.setTimeOffset(3600);

}



void loop() {
  Serial.print("SO-WI-Zeit Anfang: ");
  showTime();
  Serial.print("Epoch Time: ");
  Serial.print("SO-WI-Zeit Ende: ");

  //----------------Clock-----------------------------------------
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime(); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
  Serial.print((float)temperature - 3);
  Serial.print(" *C, ");
  Serial.print((float)humidity);
  Serial.println(" RH%");
  delay(5000); // DHT11 Abfragezeit

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


      //------- Gas Sensor MQ135 ------------
      int gassensorAnalog = analogRead(Gas_analog);
      //  int gassensorDigital = digitalRead(Gas_digital);
      delay(100);


      Serial.print("Gas Sensor: ");
      Serial.println(gassensorAnalog);
      //  Serial.println("\t");
      //  Serial.print(gassensorDigital);
      //  Serial.print("\t");
      //  Serial.print("\t");

      if (gassensorAnalog <= 550)  {
        Serial.println("=================================");
        Serial.println("kein Gas");
        Serial.println("=================================");
        digitalWrite(led_gruen, HIGH);        // OK
      } else {
        digitalWrite(led_gruen, LOW);
      }


      if (gassensorAnalog >= 551 && gassensorAnalog <= 799) {
        digitalWrite(led_gelb, HIGH);
        Serial.println("=================================");
        Serial.println("Gas erkannt");
        Serial.println("=================================");
      } else {
        digitalWrite(led_gelb, LOW);
      }



      if (gassensorAnalog > 800) {
        Serial.println("=================================");
        Serial.println("Gas-Alarm");
        Serial.println("=================================");
        digitalWrite(led_rot, HIGH);         // Gas-Alarm

        digitalWrite (Buzzer, HIGH) ; //send tone

      } else {
        digitalWrite(led_rot, LOW);
        digitalWrite (Buzzer, LOW) ;  //no tone
      }
      delay(100);
      //-----------------------------------------------------



      Serial.println("=================================");

      //********* TFT *****************
      tft.setTextSize (2);

      //---------- Seite 1 ----------------------------

      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
      tft.drawString("Datum-Uhrzeit", 5, 0);
      //  tft.drawString("Tegernsee", 22, 26);


      // tft.drawString("Temp :", 0, 52);
      tft.setCursor (42, 26);
      tft.print (weekDay);
      tft.setCursor (0, 52);
      tft.print(monthDay); tft.print(". "); tft.print (currentMonthName); tft.print(" "); tft.print (currentYear);
     

      tft.setCursor (25, 88); //20 , 88
      tft.setTextColor(0xFBE0, TFT_BLACK);
      tft.setTextSize (4);
      tft.print(tm.tm_hour);
     // tft.print(currentHour);
      tft.print(":");
      if (currentMinute < 10) {
        tft.print("0");
        tft.print(currentMinute);
      } else {
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
      tft.drawString("Wetterdaten", 12, 0);
      tft.drawString("Tegernsee", 22, 26);


      tft.drawString("Temp: ", 0, 52);
      tft.setCursor (65, 52);
      tft.print (maxtemp - 273.15, 1); tft.print ("\xF7""C");


      tft.drawString("LuDr: ", 0, 78);
      tft.setCursor (65, 78);
      tft.print (myObject["main"]["pressure"]); tft.print ("hPa");

      tft.drawString("Wind: ", 0, 104);
      tft.setCursor (65, 104);
      tft.print (myObject["wind"]["speed"]); tft.print ("m/s");

      delay(4000);
      
      //*******************************************

      //--------- Seite 3 -----------------------------
  
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
      tft.drawString("Wetterdaten", 12, 0);
      tft.drawString("Tegernsee", 22, 26);

      tft.drawString("Feuchte: ", 0, 52);
      tft.setCursor (95, 52);
      tft.print (myObject["main"]["humidity"]);
      tft.print ("%");
      
      tft.drawString("Wolken: ", 0, 78);
      tft.setCursor (95, 78);
      tft.print (myObject["clouds"]["all"]);
      tft.print ("%");    


      tft.drawString("minTemp: ", 0, 104);
      tft.setCursor (95, 104);
      tft.print (mintemp - 273.15, 1);
      tft.print ("\xF7");
 //     tft.print ("\xF7""C");    
           
      delay(4000);

      //---------- Seite 4 ----------------------------

      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
      tft.drawString("Raumklima", 22, 16);
      //  tft.drawString("Tegernsee", 22, 26);


      tft.drawString("Temp :", 0, 52);
      tft.setCursor (76, 52);
      tft.print (temperature-5,1); tft.print ("\xF7""C");


      tft.drawString("Luftf: ", 0, 78);
      tft.setCursor (76, 78);
      tft.print (humidity,1); tft.print ("%");

 //     delay(4000);      
      //*******************************************
      
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
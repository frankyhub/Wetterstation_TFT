<a name="oben"></a>

# Wetterstation_TFT
ESP32 Wetterstation mit WLAN-Manager - Wetterdaten Tegernsee und Raumklima, 1.77" TFT Display


## Story:
Diese Anleitung beschreibt den Aufbau einer Wetterstation in einem Holzgehäuse mit einem ESP32. Beim ESP32 ist ein DHT11-Sensor und einem TFT Display 1.77" angeschlossen. Die Wetterstation beinhaltet einen WiFi-Manager für die Einstellung der WLAN-Zugangsdaten (SSID und Passwort). Nach den ersten Start der Wetterstation, muss in den WLAN-Einstellungen des Handy oder Tablet das Netzwerk "Wetterstation" ausgewählt und über einen Browser die IP-Adresse 192.168.4.1 aufgerufen werden. Danach startet die "Wetterstation WLAN-Einstellung" und die Zugangsdaten für das WLAN können eingetragen werden. Sind die Zugangsdaten korrekt, startet die Wetterstation.
Die Wetterstation zeigt beim Start für 5 Sekunden einen Smiley. In dieser Zeit werden die aktuelle Uhrzeit, das Datum und die aktuellen Wetterdaten aus der Region Tegernsee von de.pool.ntp.org und von openweathermap.org geladen.
Sind die Daten geladen, verschwindet der Smiley und es werden in regelmäßigen Abständen drei Seiten mit Datum und Uhrzeit, danach das Raumklima mit Temperatur und Luftfeuchte, zuletzt die Wetterdaten Außen-Temperatur, Windgeschwindigkeit und Luftdruck angezeigt.

![Bild](/png/Wetterstation.png)

## Hardware:
Das benötigen wir für die Wetterstation:


| Anzahl | Bezeichnung | 
| -------- | -------- | 
| 1        | ESP32       |
| 1        | ESP32-Shield        | 
| 1        | ESP32 Stiftleiste       | 
| 1        | DHT11-Sensor       | 
| 1        | 10k Widerstand      | 
| 1        | TFT Display 1.77"        | 
| 1        | 8V Netzteil       | 
| 1        | Sperrholzplatte 4mm x 600x300        | 
| 1        | 3D-Druck Teile für den ESP32, das OLED-Display und den DHT11        | 
| 1        | Schaltdraht      | 
| 1        | Sekundenkleber      | 
| 1        | Holzkleber      |
| -------- | -------- | 

## Der Aufbau erfolgt in 5 Schritten:

1. Schritt Verdrahten

+ Wir löten auf das ESP32-Shield die Stiftleisten
+ Am ESP32-Shield wird ein 10kΩ Pullupwiderstand gegen +5V angelötet
+ Wir schließen mit Schaltdraht den DHT11-Sensor am ESP32-Shield GPIO27 an
+ Wir verdrahten den DHT11 mit +5V und GND
+ Wir verbinden mit Schaltdraht den ESP32 mit dem Display
+ Ist der ESP32 auf dem Shield, der Sensor und das Display angelötet, erfolgt der erste Test



| ESP32 | 1.77" TFT | 
| -------- | -------- | 
|  GND        | GND       |
|   5V      |  VCC     |
|  G18        |  SCK     |
|   G23       |  SDA (MOSI)      |
|   G14       |  RES (RST)     |
|   G13       |  RS (DC)     |
|   G12       |  CS     |
|   3.3V      |    LEDA    |
|   ---      |    ---  |



| ESP32 | 1.8" TFT | 
| -------- | -------- | 
|  GND        | GND       |
|   5V      |  VCC     |
|  G18        |  SCK     |
|   G23       |  SDA (MOSI)      |
|   G14       |  RESET     |
|   G13       |  A0     |
|   G12       |  CS     |
|   3.3V      |    LED    |
|   ---      |    ---  |


Stiftleisten einlöten:
  
![Bild](/png/espshield2.png)

DHT11 anlöten:

![Bild](/png/dht11.png)


---

2. Schritt 3D-Druckteile zeichnen und drucken

+ 3D-Druckteile zeichnen und drucken
+ 4 x ESP32-Pins d=5mm h=7mm
+ 1 x DHT11-Halterung 24x8x21 mit DHT11 Aussparung


---

3. Schritt Gehäuse lasern

+ Wir erstellen mit [BOXES.PY](https://festi.info/boxes.py/) eine Lasercutter Vorlage
+ Wähle die ClosedBox - Fully closed box mit den Maßen x=110, y=90, h=90 und thickness=4
+ Zusätzlich werden runde Öffnungen für den DHT11-Sensor in der Frontseinte,
+ eine Aussparung für das TFT-Display und
+ zwei Aussparungen für den Spannungs- und Programmieranschluss des ESP32 in der Seitenwand eingefügt


![Bild](/png/lasercutter.jpg)


---



4. Schritt Montage

+ Mit Sekundenkleber die 3D-Druck Bauteilfüße auf den ESP32 kleben
+ Das TFT-Display auf die Frontseite kleben
+ Die DHT11-Halterung an die Front-Innenseite kleben
+ Den ESP32 zu den Aussparungen der Seitenwand justieren und auf die Grundplatte kleben
+ Vor dem finalen verkleben des Gehäuses erfolgt ein Funktionstest
+ War der Funktionstest erfolgreich, kann das Gehäuse vollständig verklebt werden


![Bild](/png/wetter9.JPG)  


Erster Funktionstest am Seriellen Monitor:

![Bild](/png/wetter10.png)  

---

5. Schritt Inbetriebnahme

+ Im Handy oder Tablet die WLAN-Einstellungen öffnen
+ Das Netzwerk "Wetterstation" aufrufen
+ Im Browser die IP-Adresse 192.168.4.1 aufrufen
+ Die Seite "Wetterstation WLAN Einstellung" öffnet sich
+ Die WLAN-Zugangsdaten SSID und Passwort eingeben und speichern
+ Der Zeitserver ist voreingestellt und muss nicht verändert werden
+ Ist die WLAN-Verbindung korrekt, startet die Wetterstation mit einem Smiley am Display
+ Danach werden abwechselnd das aktuelle Datum mit Uhrzeit, danach das Raumklima mit Temperatur und Luftfeuchte und die aktuellen Wetterdaten aus der Region Tegernsee angezeigt
+ Die angezeigten Daten wechseln in regelmäßigen Abständen

Aufruf "Wetterstation" in den WLAN-Einstellungen

![Bild](/png/ws_ap_einstellung.png)  


Im Browser die IP-Adresse 192.168.4.1 öffnen, die WLAN Zugangsdaten eingeben und speichern

![Bild](/png/ws_wlaneinstellung2.png)  

Sind die WLAN-Zugangsdaten korrekt, startet die Wetterstation

![Bild](/png/ws_wlaneinstellung3.png)  

Startet die Wetterstation nicht, muss der Vorgang wiederholt werden.
Kontrolle am Seriellen Monitor:

![Bild](/png/wetter11.png)  

![Bild](/png/wetter12.png)  

Fertig, Glückwunsch!

---

## Troubleshooting, mögliche Fehlerursachen

+ Sichtkontrolle - sind alle Lötstellen ok?
+ Sind Lötbrücken zusehen oder sind Bauteilfüße fälschlicherweise mit einander verbunden?
+ Sitzt der ESP32 korrekt auf dem Shield? Auf die Polung achten und und Bauteilfüße kontrollieren.
+ Sind die Anschlussdrähte zum Sensor und Display ok?
+ Ist die Spannungsversorgung für den ESP32 ok?
+ Wurde das Programm korrekt geladen? Meldung: Hochladen abgeschlossen.
+ Wird das Display vom ESP32 erkannt? Mit dem I²C-Scanner prüfen.
+ Werden die Sensordaten und die Uhrzeit/Datum ausgelesen? Mit dem Seriellen Montitor der Arduino IDE prüfen.
+ Wurden die WLAN-Zugangsdaten korrekt eingegeben?

---

## Abkürzungen

+ TFT Display -Thin-film transistor-Display, Anzeige mit Dünnschichttransistor-Ansteuerung
+ u8g2 Library - Universal 8Bit Graphics Library
+ I²C - Inter-Integrated Circuit (Serieller Datenbus)
+ SCL - Serial Clock
+ SDA - Serial Data

---

## Setup_User.h

Die Setup_User.h in C:\Users\User\Documents\Arduino\libraries\TFT_eSPI\ kopieren!!!!!

```C++
// TFT 1.77" 1.8" Library

#define ST7735_DRIVER
#define TFT_WIDTH 128
#define TFT_HEIGHT 160
#define ST7735_REDTAB
#define TFT_BACKLIGHT_ON HIGH // HIGH or LOW are options

// ###### EDIT THE PIN NUMBERS IN THE LINES FOLLOWING TO SUIT YOUR ESP32 SETUP ######
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 17 // Chip select control pin
#define TFT_DC 2 // Data Command control pin
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

```
  


---
<div style="position:absolute; left:2cm; ">   
<ol class="breadcrumb" style="border-top: 2px solid black;border-bottom:2px solid black; height: 45px; width: 900px;"> <p align="center"><a href="#oben">nach oben</a></p></ol>
</div>

---

  




   


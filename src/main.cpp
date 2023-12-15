#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <MQ135.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFiManager.h>

// define the pin used to measure the gas sensor
#define MQ135_PIN A0
#define MQ135_PPM 100.0 

// define the pin used to control the relay
#define relayPin D5 //gpio14

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

RTC_DS3231 rtc;
MQ135 gasSensor = MQ135(MQ135_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire , -1);
float temperature = 0.0;
float humidity = 50.0;

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // initialize rtc
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // initialize relay
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // initialize display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  // display welcome message 'Gas Detector' on middle of the screen
  display.setCursor((SCREEN_WIDTH - 12 * 6) / 2, (SCREEN_HEIGHT - 8) / 2);
  display.println("Gas Detector");
  display.display();
  delay(2000);
  display.clearDisplay();
  Serial.println("initialization done");

  // get temperature and humidity
  temperature = rtc.getTemperature();

  // initialize wifi to connect to internet
  WiFiManager wifiManager;
  wifiManager.autoConnect("Gas Detector");
  Serial.println("connected to wifi");
}

void loop() {
  display.clearDisplay();
  // get current time
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // display current time on screen
  display.setCursor(10, 0);
  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.day(), DEC);
  display.print(' ');
  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.print(now.second(), DEC);

  // get gas sensor value
  float rzero = gasSensor.getRZero();
  float correctedRzero = gasSensor.getCorrectedRZero(temperature, humidity);
  float resistance = gasSensor.getResistance();
  float ppm = gasSensor.getPPM();
  float correctedPPM = gasSensor.getCorrectedPPM(temperature, humidity);
  Serial.print("RZero: ");
  Serial.println(rzero);
  Serial.print("Corrected RZero: ");
  Serial.println(correctedRzero);
  Serial.print("Resistance: ");
  Serial.println(resistance);
  Serial.print("PPM: ");
  Serial.println(ppm);
  Serial.print("Corrected PPM: ");
  Serial.println(correctedPPM);

  // display gas sensor value on screen
  display.setCursor(15, 20);
  display.print("Resistance: ");
  display.println(resistance);
  display.setCursor(15, 30);
  display.print("PPM: ");
  display.println(correctedPPM);

  // display temperature and pin state
  display.setCursor(15, 40);
  display.print("Room Temp: ");
  display.println(temperature);
  display.setCursor(15, 50);
  display.print("Relay State: ");
  display.println(digitalRead(relayPin));
  display.display();

  // turn on relay if ppm is greater than 100
  if (ppm > MQ135_PPM) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }

  delay(1000);
}
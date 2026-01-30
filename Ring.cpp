#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <qrcode.h>

// NRF24 setup
#define CE_PIN 9
#define CSN_PIN 10
const byte thisSlaveAddress[5] = {'R','x','A','A','A'};
RF24 radio(CE_PIN, CSN_PIN);

struct HealthData {
  int bpm;
  int spo2;
  float lat;
  float lng;
};

HealthData dataReceived;
bool newData = false;

// QR Code setup - Using Version 3 for smaller RAM footprint
QRCode qrcode;
uint8_t qrcodeData[100]; 

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
// Use 0x3C or 0x3D based on your specific module
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Motor setup
int motorPin = 3;

void setup() {
  Serial.begin(9600);
  // Use F() macro to save RAM on every Serial print
  Serial.println(F("Receiver Starting..."));

  // NRF24 init
  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1); 
  }
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();

  // OLED init
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Stop if OLED fails
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);
}

// ---------------- QR DISPLAY FUNCTION ----------------
void displayQRCode(String text) {
  // Version 3 QR code, ECC_LOW to keep it small and readable
  qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, text.c_str());

  int scale = 2; 
  int offsetX = (SCREEN_WIDTH - qrcode.size * scale) / 2;
  int offsetY = 2; // Move up slightly

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        display.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, SSD1306_WHITE);
      }
    }
  }
  
  display.setTextSize(1);
  display.setCursor(20, 55);
  display.print(F("Scan for Location"));
  display.display();
}

void loop() {
  if (radio.available()) {
    radio.read(&dataReceived, sizeof(dataReceived));
    newData = true;
  }

  if (newData) {
    // Serial debugging using F() macro
    Serial.print(F("BPM: ")); Serial.print(dataReceived.bpm);
    Serial.print(F(" | SpO2: ")); Serial.println(dataReceived.spo2);

    // Abnormal check
    static int abnormalCount = 0;
    bool isAbnormal = (dataReceived.bpm < 40 || dataReceived.bpm > 120 || dataReceived.spo2 < 90);

    if (isAbnormal) abnormalCount++;
    else abnormalCount = 0;

    display.clearDisplay();

    if (abnormalCount >= 3) {
      // ALERT STATE
      digitalWrite(motorPin, HIGH);
      
      if (dataReceived.lat != 0.0 && dataReceived.lng != 0.0) {
        // Correct Google Maps URL format
        String mapURL = "https://www.google.com/maps?q=" + String(dataReceived.lat, 6) + "," + String(dataReceived.lng, 6);
        displayQRCode(mapURL);
      } /*else {
        display.setCursor(0, 20);
        display.setTextSize(2);
        display.println(F("CRITICAL!"));
        display.setTextSize(1);
        display.println(F("No GPS Signal"));
        display.display();
      }*/
      
      delay(500);
      digitalWrite(motorPin, LOW);
    } 
    else {
      // NORMAL STATE
      digitalWrite(motorPin, LOW);
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println(F("HEALTH MONITOR"));
      display.drawLine(0, 12, 128, 12, SSD1306_WHITE);
      
      display.setCursor(0, 25);
      display.print(F("Heart Rate: ")); 
      display.print(dataReceived.bpm); display.println(F(" bpm"));
      
      display.setCursor(0, 40);
      display.print(F("Blood Oxygen: ")); 
      display.print(dataReceived.spo2); display.println(F("%"));
      
      display.display();
    }

    newData = false;
  }
}

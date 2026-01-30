#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <TinyGPSPlus.h>
#include "MAX30100_PulseOximeter.h"

//nRF24 Setup
#define CE_PIN 4
#define CSN_PIN 5
const byte slaveAddress[5]={'R','x','A','A','A'};
RF24 radio(CE_PIN,CSN_PIN);

//GPS Setup
TinyGPSPlus gps;
#define gpsSerial Serial2

//MAX30100 Setup
PulseOximeter pox;
uint32_t tsLastReport=0;

//Data Struct
struct HealthData
{
  int bpm;
  int spo2;
  float lat;
  float lng;
};
HealthData dataToSend;

//Setup
void setup()
{
  Serial.begin(115200);

  //NRF24 init
  if(!radio.begin())
  {
    Serial.println("NRF24 not Responding!");
    while(1);
  }
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(3,5);
  radio.openWritingPipe(slaveAddress);
  radio.stopListening();

  //GPS init
  gpsSerial.begin(9600,SERIAL_8N1,16,17);
  Serial.println("Waiting for GPS fix..");

  //MAx30100 init
  Serial.print("Initializing Max30100..");
  if
  (!pox.begin())
  {
    Serial.println("FAILED");
    while(1);
  }
  else
  {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    }
    //Loop
    void loop()
    {
      pox.update();

      //Update GPS
      while(gpsSerial.available()>0)
{
  gps.encode(gpsSerial.read());
}

//Every second,send data
if(millis()-tsLastReport>1000)  
{
  dataToSend.bpm=(int)pox.getHeartRate();
  dataToSend.spo2=(int)pox.getSpO2();
  dataToSend.lat=gps.location.isValid() ? gps.location.lat():0.0;
  dataToSend.lng=gps.location.isValid() ? gps.location.lng():0.0;

  bool rslt=radio.write(&dataToSend,sizeof(dataToSend));
  
  Serial.print("Sent-> BPM:");
  Serial.print(dataToSend.bpm);
  Serial.print("| spo2:");
  Serial.print(dataToSend.spo2);
  Serial.print("| Lat:");
  Serial.print(dataToSend.lat,6);
  Serial.print("| Lng:");
  Serial.print(dataToSend.lng,6);
  Serial.println(rslt ? "[Ack]":"[Tx Failed]");

  tsLastReport=millis();
}  
    }



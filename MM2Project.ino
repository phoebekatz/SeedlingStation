// MM2 Final Project Code
// Phoebe Katz
// Nov 19, 2024.
// reads temp every 10 min, records to data file on microSD card

#include <Adafruit_Sensor.h>
#include <Adafruit_AHTX0.h>
#include <SD.h>

#define TCAADDR 0x74

Adafruit_AHTX0 aht1;

File data;

void channelSelect(uint8_t);
void setup() 
{
  Wire.begin();

  // Serial check
  while (!Serial)
  {
    delay(10);     // will pause until serial console opens
  }

  // SD card check
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) 
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // Temp/Humidity Probe Check
  channelSelect(0);
  if (! aht1.begin()) 
  {
    Serial.println("Could not find AHT 1? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");

  data = SD.open("data.txt", FILE_WRITE);
}

void loop() 
{
  
  //TEMP/HUMIDITY PROBE 1
  sensors_event_t probeHumidity0, probeTemp0;

  channelSelect(0);
  aht1.getEvent(&probeHumidity0, &probeTemp0);// populate temp and humidity objects with fresh data
  Serial.print("Temperature 1: "); Serial.print(probeTemp0.temperature); Serial.println(" degrees C");
  Serial.print("Humidity 1: "); Serial.print(probeHumidity0.relative_humidity); Serial.println("% rH");

  while (!data) 
  {
    delay(10);
  } 

  data.println(probeTemp0.temperature);
  data.println(probeHumidity0.relative_humidity);

  delay (600000); //10 min delay
}

void channelSelect(uint8_t channel) //changing multiplexer(TCA)'s output channel
{
  Wire.beginTransmission(TCAADDR);
  Wire.write(1<<channel);
  Wire.endTransmission();
}

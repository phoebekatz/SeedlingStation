// MM2 Final Project Code
// Phoebe Katz
// Nov 19, 2024.
// just temp sensor, NO SD card or saving data

#include <Adafruit_Sensor.h>
#include <Adafruit_AHTX0.h>

#define TCAADDR 0x74

Adafruit_AHTX0 aht1;

void channelSelect(uint8_t);
void setup() 
{
  Serial.begin(115200);
  Wire.begin();

  // Ambient Temp/Humidity Check
  while (!Serial)
    delay(10);     // will pause until serial console opens

  // Temp/Humidity Probe Check 1
  channelSelect(0);
  if (! aht1.begin()) 
  {
    Serial.println("Could not find AHT 1? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
}

void loop() 
{
  
  //TEMP/HUMIDITY PROBE 1
  sensors_event_t probeHumidity0, probeTemp0;

  channelSelect(0);
  aht1.getEvent(&probeHumidity0, &probeTemp0);// populate temp and humidity objects with fresh data
  Serial.print("Temperature 1: "); Serial.print(probeTemp0.temperature); Serial.println(" degrees C");
  Serial.print("Humidity 1: "); Serial.print(probeHumidity0.relative_humidity); Serial.println("% rH");

  delay(5000);
}

void channelSelect(uint8_t channel) //changing multiplexer(TCA)'s output channel
{
  Wire.beginTransmission(TCAADDR);
  Wire.write(1<<channel);
  Wire.endTransmission();
}

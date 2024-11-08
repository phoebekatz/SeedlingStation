// Combined code, Hydroponics 2 XProject
// Phoebe Katz
// March 22, 2024

#include <Adafruit_Sensor.h>
//#include <Adafruit_SHTC3.h>
#include <Adafruit_AHTX0.h>

#define TCAADDR 0x74
//Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();
Adafruit_AHTX0 aht1;
Adafruit_AHTX0 aht2;

void channelSelect(uint8_t);
void setup() 
{
  Serial.begin(115200);
  //Serial.println("test");
  Wire.begin();
  // Ambient Temp/Humidity Check
  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  // Serial.println("SHTC3 test");
  // if (! shtc3.begin()) 
  // {
  //   Serial.println("Couldn't find SHTC3");
  //   while (1) delay(1);
  // }
  // Serial.println("Found SHTC3 sensor");

  // Temp/Humidity Probe Check 1
  channelSelect(0);
  if (! aht1.begin()) 
  {
    Serial.println("Could not find AHT 1? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");

    // Temp/Humidity Probe Check 2
  channelSelect(3);
  if (! aht2.begin()) 
  {
    Serial.println("Could not find AHT 2? Check wiring");
    delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
}

void loop() 
{
  // AMBIENT TEMP/HUMIDITY
  // sensors_event_t ambientHumidity, ambientTemp;
  
  // shtc3.getEvent(&ambientHumidity, &ambientTemp);// populate temp and humidity objects with fresh data
  
  // Serial.print("Ambient Temperature: "); Serial.print(ambientTemp.temperature); Serial.println(" degrees C");
  // Serial.print("Ambient Humidity: "); Serial.print(ambientHumidity.relative_humidity); Serial.println("% rH");
  
  //TEMP/HUMIDITY PROBE 1
  sensors_event_t probeHumidity0, probeTemp0;

  channelSelect(0);
  aht1.getEvent(&probeHumidity0, &probeTemp0);// populate temp and humidity objects with fresh data
  Serial.print("Temperature 1: "); Serial.print(probeTemp0.temperature); Serial.println(" degrees C");
  Serial.print("Humidity 1: "); Serial.print(probeHumidity0.relative_humidity); Serial.println("% rH");

  //TEMP/HUMIDITY PROBE 2
  sensors_event_t probeHumidity1, probeTemp1;
  channelSelect(3);
  aht2.getEvent(&probeHumidity1, &probeTemp1);// populate temp and humidity objects with fresh data
  Serial.print("Temperature 2: "); Serial.print(probeTemp1.temperature); Serial.println(" degrees C");
  Serial.print("Humidity 2: "); Serial.print(probeHumidity1.relative_humidity); Serial.println("% rH");
  // // LIQUID LEVEL SENSOR
  float resistance = analogRead(0); //read resistance from analog pin 0
  
  // conversion to real values
  float difference = (resistance-505)/362; //calculates percent difference from max R
  float eqResistance = 2400 - difference*1900;

  //resistance -> height
  float height = ( ( (resistance-500)/370 ) * 31.2 ) + 2;

  //Serial.print("Height: " + height + " cm");
  Serial.print("Height: ");
  Serial.print(height);
  Serial.println(" cm");

  delay(5000);
}

void channelSelect(uint8_t channel) //changing multiplexer(TCA)'s output channel
{
  Wire.beginTransmission(TCAADDR);
  Wire.write(1<<channel);
  Wire.endTransmission();
}

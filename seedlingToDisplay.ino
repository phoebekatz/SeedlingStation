// Hydroponics 2 XProject
// Phoebe Katz (phoebekatz645@gmail.com)
// March 29, 2024


//LIBRARIES
#include <Adafruit_Sensor.h>
//#include <Adafruit_SHTC3.h>
#include <Adafruit_AHTX0.h>
#include "Arduino_GigaDisplay_GFX.h"

//OBJECTS
GigaDisplay_GFX display;
Adafruit_AHTX0 aht1; //probe sensor 1, for top level temp and humidity
Adafruit_AHTX0 aht2; //probe sensor 2, for middle level temp and humidity
//Adafruit_SHTC3 shtc3 = Adafruit_SHTC3(); //ambient sensor, for bottom level Temp & humidity

#define TCAADDR 0x74 //multiplexer to allow for I2C communication to multiple sensors with the same address

//COLORS
#define BLACK 0x0000
#define GREEN 0x2aa2
#define WHITE 0xffff
#define CREAM 0xeef7
#define BLUE 0x4b17

//FUNCTIONS
void channelSelect(uint8_t);
void drawThickRoundRect(uint16_t,uint16_t,uint16_t,uint16_t,uint16_t,uint16_t, uint16_t);
void partialFillRoundRect(uint16_t,uint16_t,uint16_t,uint16_t,uint16_t,uint16_t, uint16_t);

//COUNTER
 int count = 1430; //monitors when pump should turn on

//START
void setup() 
{
  Wire.begin();

  display.begin();
  display.setRotation(1); // establish horizontal orientation of board
  display.fillScreen(GREEN);

  //draw rectangles (3 vertically stacked for levels of seedling station)
  drawThickRoundRect(10,10,370,150,25,3,BLACK);//top level 

//STOPS DRAWING ON DISPLAY AROUND HERE

  drawThickRoundRect(10,170,370,150,25,3,BLACK);//medium level
  drawThickRoundRect(10,330,370,140,25,3,BLACK);//bottom level

  //rectangles for liquid level monitoring
  //drawThickRoundRect(390,10,190,460,25,3,BLACK); //Adult plant station
  drawThickRoundRect(595,10,190,460,25,3,BLACK); // seedling station

  display.setTextSize(3); //adjust text size
  display.setTextColor(CREAM);
// Temp/Humidity Probe Check. Top Level
  channelSelect(0);
  display.setCursor(25,45); //x,y

  if (! aht1.begin()) 
  {
    display.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  display.println("AHT10 or AHT20 found");

// Temp/Humidity Probe Check. Middle Level
  channelSelect(1);
  if (! aht2.begin()) 
  {
    display.setCursor(22,205); //x,y
    display.println("Could not find AHT? Check wiring");
    delay(10); //edited this line 8/27
  }
  display.println("AHT10 or AHT20 found");

  // // Ambient Temp/Humidity Check. Bottom Level
  // display.setCursor(25,355); //x,y
  
  // display.println("SHTC3 test");
  // if (! shtc3.begin()) 
  // {
  //   display.println("Couldn't find SHTC3");
  //   while (1) delay(10);
  // }
  // display.println("Found SHTC3 sensor");
}

void loop() 
{
  //TEMP/HUMIDITY PROBE 1 (top level)
  sensors_event_t probeHumidity0, probeTemp0;
  channelSelect(0);
  aht1.getEvent(&probeHumidity0, &probeTemp0);// populate temp and humidity objects with fresh data
  display.setCursor(25,45); //x,y
  display.print("Temperature 1: "); display.print(probeTemp0.temperature); display.println(" degrees C");
  display.print("Humidity 1: "); display.print(probeHumidity0.relative_humidity); display.println("% rH");

  //TEMP/HUMIDITY PROBE 2 (middle level)
  sensors_event_t probeHumidity1, probeTemp1;
  channelSelect(1);
  aht2.getEvent(&probeHumidity1, &probeTemp1);// populate temp and humidity objects with fresh data
  display.setCursor(22,205); //x,y
  display.print("Temperature 2: "); display.print(probeTemp1.temperature); display.println(" degrees C");
  display.print("Humidity 2: "); display.print(probeHumidity1.relative_humidity); display.println("% rH");

  // // AMBIENT TEMP/HUMIDITY (bottom level)
  // sensors_event_t ambientHumidity, ambientTemp;
  
  // shtc3.getEvent(&ambientHumidity, &ambientTemp);// populate temp and humidity objects with fresh data
  //display.setCursor(25, 365); //x,y
  // display.print("Ambient Temperature: "); display.print(ambientTemp.temperature); display.println(" degrees C");
  // display.print("Ambient Humidity: "); display.print(ambientHumidity.relative_humidity); display.println("% rH");

  // LIQUID LEVEL SENSOR
  float resistance = analogRead(0); //read resistance from analog pin 0
  
  // conversion to real values
  float difference = (resistance-505)/362; //calculates percent difference from max R
  float eqResistance = 2400 - difference*1900;

  //resistance -> height
  float height = ( ( (resistance-500)/370 ) * 31.2 ) + 2;
  float percentOfTotal = height/28.0;

  //display
  partialFillRoundRect(595,10,190,460,25,14,BLUE);
  display.setCursor(630,240);
  display.println(percentOfTotal);
  display.println("% full");

  //RELAY (PUMP CONTROL)
  if (count == 1440) //turn pump on once a day
  {
    digitalWrite(2,1); //set digital pin 2 (turn on pump)
    delay(30000); // pump on for 30 s
    digitalWrite(2,0); //turn off pump

    count = 0;
  }
 count ++;

  delay(60000); //repeat loop every minute
}

void channelSelect(uint8_t channel) //changing multiplexer (TCA's) output channel
{
  Wire.beginTransmission(TCAADDR);
  Wire.write(1<<channel);
  Wire.endTransmission();
}

void drawThickRoundRect(uint16_t originX,uint16_t originY,uint16_t width,uint16_t height,uint16_t radius,uint16_t thickness, uint16_t color)
{
  for(int i = 0; i<thickness; i++)
  {
    // uint16_t x = originX + i; 
    // uint16_t width = lengthX - (2*i);
    // uint16_t height = l
    display.drawRoundRect(originX + i, originY + i, width - (2*i), height - (2*i), radius, color);
  }
}

void partialFillRoundRect(uint16_t originX,uint16_t originY,uint16_t lengthX,uint16_t lengthY,uint16_t radius,uint16_t waterHeight, uint16_t color)
{
  //originX: x-coordinate of the top left corner of the full rectangle (if not rounded)
  //originY: y-coordinate of the top left corner of the rull rectangle (if not rounded)
  //lengthX: horizontal width of the full rectangle
  //lengthY: vertical height of the full rectangle
  //waterHeight:height in centimetres as measured by the liquid level sensor (physical max = 28cm/11in)
  //color: color you want for the new rectangle (the fill)

  uint16_t height = (waterHeight * lengthY) / 28; // height, in pixels, that will be shown on the display
  uint16_t y2 = originY + ( lengthY - height);
  display.fillRoundRect(originX, y2, lengthX, height,radius,color);
  display.fillRect(originX, y2 , radius, radius, color);
  display.fillRect( (originX + lengthX) - radius, y2, radius, radius, color);
}
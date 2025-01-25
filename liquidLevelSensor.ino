//Phoebe Katz
// March 14, 2024
//Liquid Level Sensor Code

void setup() 
{
  Serial.begin(115200);
  //analogReadResolution(16);
}

void loop(void) 
{
  float resistance = analogRead(0); //read resistance from analog pin 0
  
  // conversion to real values
  float difference = (resistance-505)/362; //calculates percent difference from max R
  float eqResistance = 2400 - difference*1900;

  //resistance -> height
  float height = ( ( (resistance-500)/370 ) * 31.2 ) + 2;

  //print resistance and height
  Serial.print("Calculated Resistance: ");
  Serial.print(eqResistance); 
  Serial.println(" Ohms");

  //Serial.print("Height: " + height + " cm");
  Serial.print("Height: ");
  Serial.print(height);
  Serial.println(" cm");

  delay(3000);
}

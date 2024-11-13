#include <Arduino.h>
  const int luchtSensorPin = 4;
  const int vochtigheidSensorPin = 2;
//comment
void setup() {
pinMode(luchtSensorPin,INPUT);
pinMode(vochtigheidSensorPin,INPUT);
Serial.begin(115200);
}

void loop() {
int lucht = analogRead(luchtSensorPin);
int vochtigheid = analogRead(vochtigheidSensorPin);     //droog = 1600   in water = 700
Serial.print("vochtigheid: ");  
Serial.println(vochtigheid);
Serial.print("lucht: ");  
Serial.println(lucht);
delay(300);
}
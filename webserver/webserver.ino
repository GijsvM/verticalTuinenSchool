#include <Arduino.h>
const int vochtigheidSensorPin = 39;
const int pompPin = 25;

void setup() {
pinMode(pompPin,OUTPUT);
Serial.begin(115200);\
delay(100);
}

void loop() {
digitalWrite(pompPin,LOW);
Serial.println("uit");
delay(5000);
digitalWrite(pompPin,HIGH);
Serial.println("aan");
delay(500);
}
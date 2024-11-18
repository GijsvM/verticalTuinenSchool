#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const int luchtSensorPin = 36;
const int vochtigheidSensorPin = 39;
const int pompPin = 25;

const char* ssid = "NETLAB-OIL460";
const char* password = "Startsemester";

const char* serverURL = "http://192.168.68.92:8080/plantData";
const char* serverName = "http://192.168.68.92:8080/plantData/1";

unsigned long lastTime = 0;
unsigned long timerDelay = 11000;
unsigned long sensorReadInterval = 10000;
unsigned long pumpRuntime = 1000; // Default pump runtime in milliseconds
unsigned long lastSensorReadTime = 0;
unsigned long pumpStartTijd = 0;

String sensorReadings;
bool pumpActive = false;

String httpGETRequest(const char* serverName);

void setup() {
  pinMode(luchtSensorPin, INPUT);
  pinMode(vochtigheidSensorPin, INPUT);
  pinMode(pompPin, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to send HTTP GET request
  if (currentMillis - lastTime > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      sensorReadings = httpGETRequest(serverName);

      if (sensorReadings == "{}" || sensorReadings == "") {
        Serial.println("Empty response received, skipping JSON parsing");
      } else {
        JSONVar myObject = JSON.parse(sensorReadings);

        if (JSON.typeof(myObject) == "undefined") {
          Serial.println("Parsing input failed!");
        } else {
          JSONVar keys = myObject.keys();

          for (int i = 0; i < keys.length(); i++) {
            const char* key = (const char*)keys[i];
            JSONVar value = myObject[key];

            Serial.print("Key: ");
            Serial.print(key);
            Serial.print(" Value: ");
            Serial.println(JSON.stringify(value));

            // Check for "pumpRuntime" and update the runtime
            if (strcmp(key, "pumpRuntime") == 0) {
              pumpRuntime = (unsigned long)((int)value); // Update runtime
              Serial.print("Updated pump runtime to: ");
              Serial.println(pumpRuntime);
            }

            // Check for "message" and activate pump if the value is 1
            else if (strcmp(key, "message") == 0) {
              int messageValue = (int)value; // Explicit cast to int
              if (messageValue == 1) {
                pumpActive = true;
                pumpStartTijd = currentMillis;
                digitalWrite(pompPin, HIGH);
                Serial.println("Pump activated.");
              } else {
                digitalWrite(pompPin, LOW);
                pumpActive = false;
                Serial.println("Pump deactivated from server.");
              }
            }
          }
        }
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = currentMillis;
  }

  // Isolated pump logic based on runtime
  if (pumpActive) {
    unsigned long elapsedTime = currentMillis - pumpStartTijd;
    Serial.print("Elapsed Time: ");
    Serial.println(elapsedTime);
    Serial.print("Pump Runtime: ");
    Serial.println(pumpRuntime);

    if (elapsedTime >= pumpRuntime) {
      digitalWrite(pompPin, LOW);
      pumpActive = false;
      Serial.println("Pump deactivated after runtime.");
    }
  }

  // Periodic sensor readings and POST request
  if (currentMillis - lastSensorReadTime >= sensorReadInterval) {
    int lucht = analogRead(luchtSensorPin);
    int vochtigheid = analogRead(vochtigheidSensorPin);  // dry = 1600, in water = 700
    Serial.print("Humidity: ");  
    Serial.println(vochtigheid);
    Serial.print("Air quality: ");  
    Serial.println(lucht, DEC);
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverURL);
      http.addHeader("Content-Type", "application/json");

      String postData = "{\"plantID\": 1, \"vochtigheid\": " + String(vochtigheid) + ", \"luchtKwaliteid\": " + String(lucht) + " }";
      int httpResponseCode = http.POST(postData);
      Serial.println("POST data: " + postData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
        
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastSensorReadTime = currentMillis;
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode > 0) {
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  return payload;
}

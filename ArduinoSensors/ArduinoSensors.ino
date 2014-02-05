char command;

// HC-SR04 sonar
#include <NewPing.h>
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// BH1750 light sensor
// SDA -> #2
// SCL -> #3
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;

void readSonar() {
  unsigned int uS = sonar.ping();
  Serial.println(uS / US_ROUNDTRIP_CM);
}

void readLight() {
  uint16_t lux = lightMeter.readLightLevel();
  Serial.println(lux);
}

void setup() {
  Serial.begin(9600);
  lightMeter.begin();
}

void loop() {
  while (Serial.available() > 0) {      
      command = Serial.read();
      
      switch (command) {
        case 's':
        case 'S':
          readSonar();
          break;
          
        case 'l':
        case 'L':
          readLight();
          break;
      } 
  }
}

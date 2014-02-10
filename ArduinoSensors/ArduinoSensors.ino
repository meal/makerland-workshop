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
#include "BH1750.h"
BH1750 lightMeter;

// PIR HC-SR501
// the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 15;
boolean isCalibrated = false;
int pirPin = 5;

// DHT11 temperature/humidity sensor
#include "Dht11.h"
int DHT_DATA_PIN = 7; // The data I/O pin connected to the DHT11 sensor
static Dht11 sensor(DHT_DATA_PIN);

// Diode setup
int diodePin = 13;

void setup() {
  Serial.begin(9600);
  
  lightMeter.begin();
  
  pinMode(pirPin, INPUT);
  
  pinMode(DHT_DATA_PIN, INPUT);
  digitalWrite(DHT_DATA_PIN, HIGH);
  
  pinMode(diodePin, OUTPUT);
}

void readSonar() {
  unsigned int uS = sonar.ping();
  Serial.println(uS / US_ROUNDTRIP_CM);
}

void readLight() {
  uint16_t lux = lightMeter.readLightLevel();
  Serial.println(lux);
}

void readMotion() {
  if (!isCalibrated) {
    Serial.println("CALIBRATING");
    digitalWrite(pirPin, LOW);
    delay(1000 * calibrationTime);
    isCalibrated = true;
  }
  Serial.println(digitalRead(pirPin) ? "1" : "0");
}

void setDiode() {
  while (Serial.available() == 0) {}
  digitalWrite(diodePin, Serial.read() == '1');
}

void readTemperature() {
  switch (sensor.read()) {
      case Dht11::OK:
          Serial.println(sensor.getTemperature());
          break;
  
      case Dht11::ERROR_CHECKSUM:
          Serial.println("-1");
          break;
  
      case Dht11::ERROR_TIMEOUT:
          Serial.println("-2");
          break;
  
      default:
          Serial.println("-3");
          break;
    }
}

void readHumidity() {
  switch (sensor.read()) {
      case Dht11::OK:
          Serial.println(sensor.getHumidity());
          break;
  
      case Dht11::ERROR_CHECKSUM:
          Serial.println("-1");
          break;
  
      case Dht11::ERROR_TIMEOUT:
          Serial.println("-2");
          break;
  
      default:
          Serial.println("-3");
          break;
    }
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
          
        case 'm':
        case 'M':
          readMotion();
          break;
          
        case 't':
        case 'T':
          readTemperature();
          break;          
                
        case 'h':
        case 'H':
          readHumidity();
          break;            
          
        case 'd':
        case 'D':
          setDiode();
          break;             
      } 
  }
}

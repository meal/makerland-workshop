/****************************
 * HC-SR04 sonar            *
 ****************************/
// Include sonar library.
#include <NewPing.h>
// Arduino pin connected to trigger pin on the ultrasonic sensor.
#define TRIGGER_PIN  12
// Arduino pin connected to echo pin on the ultrasonic sensor.
#define ECHO_PIN     11
// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define MAX_DISTANCE 200 
// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 


/****************************
 * BH1750 light sensor      *
 ****************************/
// Include I2C (AKA Two Wire Interface) library.
#include <Wire.h>
// Include BH1750 library.
#include "BH1750.h"
// Setup library.
BH1750 lightMeter;


/****************************
 * PIR HC-SR501             *
 ****************************/
// The time we give the sensor to calibrate (10-60 secs according to the datasheet).
int calibrationTime = 15;
// Hold the information about calibration so it's done only once after powering on.
boolean isCalibrated = false;
// Arduino pin connected to the PIR sensor.
#define PIR_PIN     5


/*************************************
 * DHT11 temperature/humidity sensor *
 *************************************/
// Include DHT11 library.
#include "Dht11.h"
// Arduino pin connected to the DHT11 sensor.
int DHT_DATA_PIN = 7;
// Setup library.
static Dht11 sensor(DHT_DATA_PIN);


/****************************
 * VCNL proximity/light sensor      *
 ****************************/
// Include VCNL4000 library.
#include "vcnl4000.h"
// Setup library.
VCNL4000 proximitySensor;
bool wasInitialized = false;


/****************************
 * Onboard LED              *
 ****************************/
// Arduino pin connected to the soldered LED.
int LED_PIN = 13;

// Variable which holds last sent command
char command;


/***********************************************************
 * Setup function.                                         *
 *                                                         *
 * This is called only once after powering on the Arduino. *
 ***********************************************************/
void setup() {
  // Setup serial port to speed of 9600 bauds
  Serial.begin(9600);
  
  // Initialize BH1750 light sensor
  lightMeter.begin();
  
  // Set PIR pin as a input one
  pinMode(PIR_PIN, INPUT);
  
  // Set BH1750 pin a input one...
  pinMode(DHT_DATA_PIN, INPUT);
  // And "pull-up" to VCC 
  digitalWrite(DHT_DATA_PIN, HIGH);
  
  // Set LED pin as a output one
  pinMode(LED_PIN, OUTPUT);
}


/***********************************************************
 * Sonar function.                                         *
 *                                                         *
 * Reads distance value and prints it to serial            *
 ***********************************************************/
void readSonar() {
  // Get the value...
  unsigned int uS = sonar.ping();
  // ...calculate the distance and print it on serial port
  Serial.println(uS / US_ROUNDTRIP_CM);
}


/***********************************************************
 * Light function.                                         *
 *                                                         *
 * Reads light level and prints it to serial               *
 ***********************************************************/
void readLight() {
  // Get the value...
  uint16_t lux = lightMeter.readLightLevel();
  // ...and print it on serial
  Serial.println(lux);
}


/***********************************************************
 * Motion function.                                        *
 *                                                         *
 * Checks if sensor is calibrated and then reads the value *
 * After powering on, the PIR sensor needs around 15       *
 * seconds to calibrate. We'll stop and wait until it's    *
 * done.                                                   *
 ***********************************************************/
void readMotion() {
  // Check if sensor was already calibrated
  if (!isCalibrated) {
    // It wasn't so inform PC about calibration via serial
    Serial.println("CALIBRATING");
    // Set PIR pin to low, which makes it calibrate
    digitalWrite(PIR_PIN, LOW);
    // Wait...
    delay(1000 * calibrationTime);
    // ...and store information about calibration for later
    isCalibrated = true;
  }
  // Read PIR value and print it on serial
  // TIP: PIR has a very long lag to go from 1 to 0 (around 3 seconds)
  Serial.println(digitalRead(PIR_PIN) ? "1" : "0");
}


/***********************************************************
 * Diode function.                                         *
 *                                                         *
 * Checks which state LED should have and sets it.         *
 ***********************************************************/
void setDiode() {
  // Wait on serial for any data (this will stop all code)
  while (Serial.available() == 0) {}
  // Get the character containing LED state ("0" or "1")
  char stateCharacter = Serial.read();
  // Set state to 1 if character is "1" and to 0 if otherwise
  boolean state = stateCharacter == '1';
  // Set te LED state
  digitalWrite(LED_PIN, state);
}


/***********************************************************
 * Temperature function.                                   *
 *                                                         *
 * Checks sensor state and prints it on serial             *
 ***********************************************************/
void readTemperature() {
  // Read DHT11 status
  switch (sensor.read()) {
      // Everything is OK, we can check the temperature
      case Dht11::OK:
          // Print the temperature
          Serial.println(sensor.getTemperature());
          break;
  
      // There was an error while checking checksum
      case Dht11::ERROR_CHECKSUM:
          // Nothing we can do about it, so we'll report impossible negative temperature
          Serial.println("-1000");
          break;
  
      // Sensor is not reponding (check the wires?)
      case Dht11::ERROR_TIMEOUT:
          // Print even lower impossible temperature
          Serial.println("-2000");
          break;
      
      // If sensor is in some other state
      default:
          // We don't know what to do about it, so: negative temperature
          Serial.println("-3000");
          break;
    }
}


/***********************************************************
 * Humidity function.                                      *
 *                                                         *
 * Checks sensor state and prints it on serial             *
 * Basically the same as temperature.                      *
 *                                                         *
 * Note: don't check humidity right after temperature.     *
 * Give the sensor about 300ms using delay function.       *
 ***********************************************************/
void readHumidity() {
  // Read DHT11 status  
  switch (sensor.read()) {
      // Everything is OK, we can check the humidity
      case Dht11::OK:
          // Print the humidity
          Serial.println(sensor.getHumidity());
          break;
  
      // There was an error while checking checksum
      case Dht11::ERROR_CHECKSUM:
          // Nothing we can do about it, so we'll report impossible negative humidity
          Serial.println("-1000");
          break;
  
      // Sensor is not reponding (check the wires?)
      case Dht11::ERROR_TIMEOUT:
          // Print even lower impossible humidity
          Serial.println("-2000");
          break;
  
      // If sensor is in some other state
      default:
          // We don't know what to do about it, so: negative temperature
          Serial.println("-3000");
          break;
    }
}

void initializeProximitySensor() {
  if (!wasInitialized) {
    // Initialize VCNL4000 proximity/light sensor
    proximitySensor.begin();
    wasInitialized = true;
  }
}

/***********************************************************
 * Ambient light function.                                 *
 *                                                         *
 * Reads ambient light value and prints it to serial       *
 ***********************************************************/
void readAmbientLight() {
  initializeProximitySensor();
  // Get the value...
  uint16_t ambientLight = proximitySensor.readAmbientData();
  // ...and print it on serial
  Serial.println(ambientLight);
}


/***********************************************************
 * Proximity function.                                     *
 *                                                         *
 * Reads proximity and prints it to serial                 *
 ***********************************************************/
void readProximity() {
  initializeProximitySensor();
  // Get the value...
  uint16_t proximity = proximitySensor.readProximityData();
  // ...and print it on serial
  Serial.println(proximity);
}


/***********************************************************
 * Loop function.                                          *
 *                                                         *
 * This is called indefinitely after setup function.       *
 ***********************************************************/
void loop() {
  // Wait for data on serial
  while (Serial.available() > 0) {      
      // Read one character
      command = Serial.read();
      
      // Test which command to perform
      switch (command) {
        // Lower or upper "S" for sonar
        case 's':
        case 'S':
          readSonar();
          break;
        
        // Lower or upper "L" for light
        case 'l':
        case 'L':
          readLight();
          break;
        
        // Lower or upper "M" for motion
        case 'm':
        case 'M':
          readMotion();
          break;
        
        // Lower or upper "T" for temperature
        case 't':
        case 'T':
          readTemperature();
          break;          
        
        // Lower or upper "H" for humidity
        case 'h':
        case 'H':
          readHumidity();
          break;            
        
        // Lower or upper "A" for humidity
        case 'a':
        case 'A':
          readAmbientLight();
          break; 
          
        // Lower or upper "P" for humidity
        case 'p':
        case 'P':
          readProximity();
          break; 
        
        // Lower or upper "D" for LED diode
        case 'd':
        case 'D':
          setDiode();
          break;             
      } 
  }
}

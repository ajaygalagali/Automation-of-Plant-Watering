/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  =>
  =>          USB HOWTO: http://tiny.cc/BlynkUSB
  =>

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT DebugSerial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(0, 1); // RX, TX

#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6L5g7XyJ4MHhr_C_tf-FqEHY9i0ppmPT";

// ma: Soil Moisture Sensor - A Variables
const int MOISTURE_SENSOR_A_PORT = A1;
int maRawValue = 0;
int maPercentage = 0;


// mb: Soil Moisture Sensor - B Variables
const int MOISTURE_SENSOR_B_PORT = A2; // Port Number
int mbRawValue = 0;
int mbPercentage = 0;

const int MOISTURE_SENSOR_MAX = 630; // Dry 500, Air:630
const int MOISTURE_SENSOR_MIN = 260; // Water
const int MOISTURE_SENSOR_THRESHOLD = 63; // If less, water plant

// wd: WaterDepth Sensor Variables
int DEPTH_SENSOR_PORT = A3; // Port Number
int wdRawValue = 0;
// Sensor value depends on type of water
const int DEPTH_HIGH = 300;
const int DEPTH_LOW = 260;

// Water Pump
const int WATER_PUMP_A_PORT = 8;
const int WATER_PUMP_B_PORT = 9;
const int WATER_FLOW_TIME = 100; // In milli seconds

// Constants
const int DELAY_ONE = 1000;


BlynkTimer timer; // Announcing the timer
void setup()
{
  // Debug console
  DebugSerial.begin(9600);

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  
  timer.run(); // running timer every second

}

int readMoistureA() {
  maRawValue = analogRead(MOISTURE_SENSOR_A_PORT);
  maPercentage = mapMoistureValue(maRawValue);

//  Serial.print("Reading Moisture A");
//  Serial.println(maPercentage);
  return maPercentage;
}

int readMoistureB() {
  mbRawValue = analogRead(MOISTURE_SENSOR_B_PORT);
  mbPercentage = mapMoistureValue(mbRawValue);

//  Serial.print("Reading Moisture B: ");
//  Serial.println(mbPercentage); 
  return mbPercentage;
}

int readWaterDepth() {
  wdRawValue = analogRead(DEPTH_SENSOR_PORT);

//  Serial.print("Reading Water Depth: ");
//  Serial.println(wdRawValue);
  return wdRawValue;

}

bool isWaterSource() {
  
  if (wdRawValue < DEPTH_LOW) {
    Serial.println("isWaterSource : FALSE");
    return false;
  }
  Serial.println("isWaterSource : TRUE");
  return true;
}

void waterPlantA() {
  Serial.println("Watering Plant A");
  digitalWrite(WATER_PUMP_A_PORT, HIGH);
  delay(WATER_FLOW_TIME);
  digitalWrite(WATER_PUMP_A_PORT, LOW);

}

void waterPlantB() {
  Serial.println("Watering Plant B");
  digitalWrite(WATER_PUMP_B_PORT, HIGH);
  delay(WATER_FLOW_TIME);
  digitalWrite(WATER_PUMP_B_PORT, LOW);
}

// Maps moisture sensor value to 0 to 100 %
int mapMoistureValue(int rawValue) {

  return map(rawValue, MOISTURE_SENSOR_MIN, MOISTURE_SENSOR_MAX, 100, 0);

}



void myTimerEvent()
{
  
  maPercentage = readMoistureA();
  Blynk.virtualWrite(V1, maPercentage);

  mbPercentage = readMoistureB();
  Blynk.virtualWrite(V2,mbPercentage);

  wdRawValue = readWaterDepth();
  Blynk.virtualWrite(V3,wdRawValue);

//  Serial.println("Sent data to Blynk");
}
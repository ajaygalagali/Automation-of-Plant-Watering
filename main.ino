

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
int wdPercentage = 0;

const int DEPTH_SENSOR_MIN = 0;
const int DEPTH_SENSOR_MAX = 0;
const int DEPTH_SENSOR_THRESHOLD = 10;

// Water Pump
const int WATER_PUMP_A_PORT = 2;
const int WATER_PUMP_B_PORT = 3;
const int WATER_FLOW_TIME = 1000; // In milli seconds

// Constants
const int DELAY_ONE = 1000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Lets get started....");

}

void loop() {
  // put your main code here, to run repeatedly:

  // Checking Water Source Level
  if (isWaterSource) {

    // Plant A
    if (readMoistureA() < MOISTURE_SENSOR_THRESHOLD) {
      // Water Plant A
      waterPlantA();

      // Notify user : Watered Plant A at <TimeStamp>

    } else {
      // Notify User: Plant A does not require water at <Timestamp>
    }
    delay(DELAY_ONE);

    // Plant B
    if (readMoistureB() < MOISTURE_SENSOR_THRESHOLD) {
      // Water Plant B
      waterPlantB();
      // Notify user : Watered Plant B at <TimeStamp>

    } else {
      // Notify User: Plant B does not require water at <Timestamp>
    }


  } else {
    // Notify User: Insufficient water source, please refill.

  }


}

int readMoistureA() {
  maRawValue = analogRead(MOISTURE_SENSOR_A_PORT);
  maPercentage = mapMoistureValue(maRawValue);

  return maPercentage;
}

int readMoistureB() {
  mbRawValue = analogRead(MOISTURE_SENSOR_B_PORT);
  mbPercentage = mapMoistureValue(mbRawValue);

  return mbPercentage;
}

int readWaterDepth() {
  wdRawValue = analogRead(DEPTH_SENSOR_PORT);
  wdPercentage = mapWaterDepthValue(wdRawValue);

  return wdPercentage;

}

bool isWaterSource() {
  if (readWaterDepth() < DEPTH_SENSOR_THRESHOLD) {
    return false;
  }

  return true;
}

void waterPlantA() {
  digitalWrite(WATER_PUMP_A_PORT, HIGH);
  delay(WATER_FLOW_TIME);
  digitalWrite(WATER_PUMP_A_PORT, LOW);

}

void waterPlantB() {
  digitalWrite(WATER_PUMP_B_PORT, HIGH);
  delay(WATER_FLOW_TIME);
  digitalWrite(WATER_PUMP_B_PORT, LOW);
}

// Maps moisture sensor value to 0 to 100 %
int mapMoistureValue(int rawValue) {

  /*
     Soil Moisture Sensor Values
    630+: Air (Sensor is not inserted into the soil)
    425 to 450: Dry Soil
    270: Wet soil
    260: Plane water without soil
    250: Very wet soil
  */

  /*
     1% = Air
     63% = Dry soil
     90% = Wet Soil
  */

  return map(rawValue, mMinValue, mMaxValue, 100, 0);

}


// Maps water depth sensor value to 0 to 100 %
int mapWaterDepthValue(int rawValue) {
  return map(rawValue, wdMin, wdMax, 0, 100);
}

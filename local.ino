

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
const int WATER_PUMP_A_PORT = 2;
const int WATER_PUMP_B_PORT = 3;
const int WATER_FLOW_TIME = 500; // In milli seconds

// Constants
const int DELAY_ONE = 1000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Lets get started....");

}

void loop() {

  // Checking Water Source Level
  if (isWaterSource()) {

      delay(DELAY_ONE);
    // Plant A
    if (readMoistureA() < MOISTURE_SENSOR_THRESHOLD) {

      delay(DELAY_ONE);
      // Water Plant A
      waterPlantA();

      // Notify user : Watered Plant A at <TimeStamp>

    } else {
      // Notify User: Plant A does not require water at <Timestamp>
    }
    delay(DELAY_ONE);

    // Plant B
    if (readMoistureB() < MOISTURE_SENSOR_THRESHOLD) {
      
      delay(DELAY_ONE);
      // Water Plant B
      waterPlantB();
      // Notify user : Watered Plant B at <TimeStamp>

    } else {
      // Notify User: Plant B does not require water at <Timestamp>
      Serial.println("Notify User: Plant B does not require water at <Timestamp>");
    }


  } else {
    // Notify User: Insufficient water source, please refill.
    Serial.println("Notify User: Insufficient water source, please refill.");

  }
  Serial.println("---------------------END of CYCLE---------------------");
  delay(1000);


}

int readMoistureA() {
  maRawValue = analogRead(MOISTURE_SENSOR_A_PORT);
  maPercentage = mapMoistureValue(maRawValue);

  Serial.print("Reading Moisture A");
  Serial.println(maPercentage);
  return maPercentage;
}

int readMoistureB() {
  mbRawValue = analogRead(MOISTURE_SENSOR_B_PORT);
  mbPercentage = mapMoistureValue(mbRawValue);

  Serial.print("Reading Moisture B: ");
  Serial.println(mbPercentage); 
  return mbPercentage;
}

int readWaterDepth() {
  wdRawValue = analogRead(DEPTH_SENSOR_PORT);

  Serial.print("Reading Water Depth: ");
  Serial.println(wdRawValue);
  return wdRawValue;

}

bool isWaterSource() {
  
  if (readWaterDepth() < DEPTH_LOW) {
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

  return map(rawValue, MOISTURE_SENSOR_MIN, MOISTURE_SENSOR_MAX, 100, 0);

}
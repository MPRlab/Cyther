// Read and Print Photocell Analog Input

// Define Constants
#define BAUDRATE 115200

// Define Pins
int photocellPin = A1;

// Define Variables
float light_reading;
float light_voltage;
//unsigned long time;

void setup() {
    Serial.begin(BAUDRATE);
}

void loop() {
  // Determine time (in uSec) since Arduino started running
  //time=micros();
  // Read photocell
  light_reading=analogRead(photocellPin);
  light_voltage=light_reading/1023;
  //Serial.write(time);
  //Serial.write(light_voltage);
  Serial.print(light_voltage);
}

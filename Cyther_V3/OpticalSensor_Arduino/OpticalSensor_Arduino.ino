// Read and Print Photocell Analog Input

// Define Constants & Pin
#define BAUDRATE 115200
#define photocellPin A9

// Define Variables
float light_reading;
int light_voltage;
//unsigned long time;

void setup() {
    Serial.begin(BAUDRATE);
    pinMode(photocellPin,INPUT);
}

void loop() {
  // Determine time (in uSec) since Arduino started running
  //time=micros();
  // Read photocell
  light_reading=analogRead(photocellPin);
  //light_voltage=light_reading/1023;
  //Serial.write(time);
  //Serial.write(light_voltage);
  Serial.println(light_reading);
}

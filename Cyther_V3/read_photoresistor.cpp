#include "mbed.h"
 
AnalogIn photocell(A0);
PwmOut myled(LED1);
Serial serial (USBTX, USBRX);

//Read and Print Photo Cell Analog Input
//Open SerialPlot on COM4 to view serial output
//Select Data Format: ASCII
int main()
{
    float light_reading;
    float light_voltage;
    serial.baud(115200);
    
    // Create timer
    Timer Write_time;
    Write_time.start();
       
    while(1) {
        light_reading=photocell.read();
        //Value from 0-1 (Full ADC Conversion Range)
        light_voltage=light_reading*5000;
        //"Light Level (mV) is: 
        serial.printf("%.0f, ", Write_time.read());
        serial.printf("%.0f \n", light_voltage);
    }}

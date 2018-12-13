#include "mbed.h"
 
AnalogIn photocell(A0);
Serial serial (USBTX, USBRX);

int main()
{
    int array_size=50000;
    int time[array_size];
    float voltage [array_size];
    //int sample_frequency=44100;
    //float delay=1/sample_frequency;
    serial.baud(230400);
    
    // Create timer
    Timer t; t.start();
    
    for(int i=1; i<=array_size; i++) {
        time[i]=t.read_us();
        voltage[i]=photocell.read()*5000;
        //wait(delay);
        }
        
    for(int i=1; i<=array_size; i++) {
        serial.printf("%i, %f\n",time[i],voltage[i]);}
    }
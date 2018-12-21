#include "mbed.h"
#include "arm_math.h"
#include "arm_common_tables.h"

/*  Just added CMSIS_DSP_5 lib.
    Implement Real FFT per 
    http://www.keil.com/pack/doc/CMSIS/DSP/html/group__RealFFT.html
    
    Code runs successfully, but how is the output formatted?
    
    Use this as a starting point for understanding output
    https://stackoverflow.com/questions/42299932/dsp-libraries-rfft-strange-results
    
    Output Notes:    
        1. 4096 floats where half are real, half are imaginary
        2. (real[1], imag[1]) is the fundamental frequency, (real[2], imag[2])
             is the first harmonic and so on
        3. Each of those frequency components are given by k*fs/N, 
            where N is the number of samples (in your case l_probek)
            and fs = 1/dt is the sampling rate
*/
AnalogIn photocell(A0);
Serial serial (USBTX, USBRX);

int main()
{
    //Initialize variables
    int array_size=4096; //65536=2^16, 4096=2^12
    int time[array_size];
    float32_t voltage[array_size];
    float32_t output[array_size];
    float32_t output_mag[array_size];
    arm_rfft_fast_instance_f32 S;
    arm_rfft_fast_init_f32(&S, array_size);
    
    serial.baud(230400);
    
    // Create timer
    Timer t; t.start();
    
    //Read time & voltage levels and store in array
    for(int i=1; i<=array_size; i++) {
        time[i]=t.read_us(); //microseconds since t.start
        voltage[i]=photocell.read()*5000; //millivolts
        //voltage[i] = (float32_t) 15*sin(2*3.14159265359*5000*i*0.000000390625);
        }
            
    //Compute real FFT of floating point    
    arm_rfft_fast_f32(&S,       //points to an arm_rfft_fast_instance_f32 structure
                      voltage,  //points to the input buffer.  Input buffer gets modified during this calculation.
                      output,   //points to the output buffer
                      0);       //RFFT if flag is 0, RIFFT if flag is 1  
            
    serial.printf("FFT Real/Complex values\n");
    //Print FFT output values
    for(int i=1; i<=array_size; i++) {
        serial.printf("%f\n",output[i]);}
        
    //Compute magnitudes
    arm_cmplx_mag_f32(output+2,
                      output_mag+1,
                      array_size/2-1);
    // Handle special cases
    output_mag[0]=output[0];
    output_mag[array_size/2]=output[1];
    
    serial.printf("FFT Magnitude values\n");    
    //Print FFT output magnitude values
    for(int i=1; i<=array_size; i++) {
        serial.printf("%f\n",output_mag[i]);}

    }
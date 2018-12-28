#include "mbed.h"
#include "arm_math.h"
#include "arm_common_tables.h"

AnalogIn photocell(A0);
Serial serial(USBTX, USBRX);

int main()
{
	//Initialize variables
	uint16_t array_size = 1024; //4096=2^12
	uint32_t block_size = 1024;
	int time[array_size];
	float32_t voltage[array_size];
	float32_t output[array_size];
	float32_t output_mag[array_size];
	float32_t max_value;
	float32_t sample_frequency;
	float32_t string_frequency;
	uint32_t max_index;
	uint8_t ifftFlag = 0;
	arm_rfft_fast_instance_f32 S;
	arm_rfft_fast_init_f32(&S, array_size);

	serial.baud(230400);

	// Create timer
	Timer t; t.start();

	serial.printf("Voltage:\n");

	//Read time & voltage levels and store in array
	for (int i = 0; i < array_size; i++) {
		time[i] = t.read_us(); //microseconds since t.start
		voltage[i] = photocell.read() * 5000; //millivolts
		//voltage[i] = 1.2f*arm_sin_f32(2*3.1415926f*50*i/1000)+1;

		serial.printf("%f\n", voltage[i]);
	}

	//Compute real FFT of floating point    
	arm_rfft_fast_f32(&S,        //arm_rfft_fast_instance_f32 structure
		voltage,   //input buffer. Gets modified during this calculation.
		output,    //output buffer
		ifftFlag); //RFFT if flag is 0, RIFFT if flag is 1  

	serial.printf("FFT Real/Complex values:\n");
	//Print FFT output values
	for (int i = 1; i <= array_size; i++) {
		serial.printf("%f\n", output[i]);
	}

	//Compute magnitudes
	arm_cmplx_mag_f32(output,
		output_mag,
		array_size / 2 - 1); //Number of samples in input vector

//Handle special cases
	output_mag[0] = 0; //output[0];
	output_mag[array_size / 2] = output[1];

	serial.printf("FFT Magnitude values:\n");
	//Print FFT output magnitude values
	for (int i = 1; i <= array_size; i++) {
		serial.printf("Time: %i, Magnitude: %f\n", time[i], output_mag[i]);
	}

	//Find max bin and location of max (first half of bins as this is the only valid section)
	arm_max_f32(output_mag, block_size / 2, &max_value, &max_index);
	serial.printf("Signal Amplitude is %f at index %i.\n", max_value, max_index);

	//Calculate frequency value
	sample_frequency = 1 / (0.000001*(time[3] - time[2]));
	string_frequency = max_index * sample_frequency / array_size;
	serial.printf("String frequency is %f Hz.\n", string_frequency);
}
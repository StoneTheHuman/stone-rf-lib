#ifndef RFReceiveConf
#define RFReceiveConf

/*
This file contains the configuration necessary to make the library work for your device. Before you edit this file to match your needs, it might be wise to make a copy of the original.

For this library to work properly, the data pin of your receiver module should be connected to the INT0 pin of your AVR-device.

// something about beginListening, pause and resume

*/


/*********************************************************************/


#define MCU attiny85		// currently supported: attiny85 and atmega328p
#define F_CPU 1000000		// currently supported: 1 MHz and 16 MHz

#define PULSE_LENGTH 600	// Length of 1 pulse in microseconds; the maximum pulselength for the current timer prescaler settings is 1600 us 
#define MAX_DEV 5 			// Maximum deviation of the received pulse length in timer ticks; if you receive inaccurate rf signals make this larger; if you receive to much noise make this smaller.


/*********************************************************************/

#endif

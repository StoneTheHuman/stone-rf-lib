#ifndef RFReceiveConf
#define RFReceiveConf


/*********************************************************************/


#define MCU attiny85		// currently supported: attiny85 and atmega328p
#define F_CPU 1000000		// currently supported: 1 MHz and 16 MHz

#define PULSE_LENGTH 600	// Length of 1 pulse in microseconds; the maximum pulselength for the current timer prescaler settings is 1600 us 
#define MAX_DEV 5 			// Maximum deviation of the received pulse length in timer ticks; if you receive inaccurate rf signals make this larger; if you receive to much noise make this smaller.


/*********************************************************************/

#endif

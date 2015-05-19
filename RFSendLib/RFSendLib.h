#ifndef RFSendLib
#define RFSendLib

#include "RFSendConf.h"

#ifndef F_CPU
#error "F_CPU needs to be defined for this shit."
#endif

#define atmega328p 2
#define attiny85 3

#if MCU == attiny85
	#define TIMSK0 TIMSK
#endif

#if F_CPU == 1000000
#define TIM0PRESCALER ((1 << CS01) | (1 << CS00)) 	// prescaler 64; fcpu 1000000 = 1/64 = 1/64 ticks per microsecond = 64 microseconds per tick
#define TIMER_US_PT 64								// Microseconds per tick
#elif F_CPU == 16000000
#define TIM0PRESCALER ((1 << CS02) | (1 << CS00)) 	// prescaler 1024; fcpu 16000000 = 16/1024 = 1/64 ticks per microsecond = 64 microseconds per tick
#define TIMER_US_PT 64								// Microseconds per tick
#endif

#define PULSE_LENGTH_T (PULSE_LENGTH/TIMER_US_PT)	// Pulse length in timer ticks

typedef struct {
	uint16_t * queue;
	uint8_t size;
	uint8_t usedSize;
} Buffer;

void writeByte(uint8_t byte);	// TODO implement code

void initTimer();
void initRFSend();
void initPin();

void setHigh();
void setLow();

void enqueue(uint16_t newData);
void dequeue();

void setZeroDelay();
void setOneDelay();




#endif // RFSendLib

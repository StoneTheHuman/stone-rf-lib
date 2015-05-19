#ifndef RFReceiveLib
#define RFReceiveLib

#include "RFReceiveConf.h"

#ifndef F_CPU
#warning "F_CPU needs to be defined for this shit. Now using 1MHz."
#define F_CPU 1000000
#endif

#define atmega328p 2
#define attiny85 3

#if MCU == attiny85
	#define TIMSK0 TIMSK
	#define COMPARE_VALUE (PINB & (1 << PB2))
	#define EIMSK GIMSK
	#define EICRA MCUCR
#endif
#if MCU == atmega328p
	#define INTERRUPT_VECTOR INT0_vect
	#define COMPARE_VALUE (PIND & (1 << PD2))
#endif
#define INTERRUPT_VECTOR INT0_vect

#if F_CPU == 1000000
	#define TIM0PRESCALER ((1 << CS01) | (1 << CS00)) 	// prescaler 64; fcpu 1000000 = 1/64 = 1/64 ticks per microsecond = 64 microseconds per tick
	#define TIMER_US_PT 64								// Microseconds per tick
#elif F_CPU == 16000000
	#define TIM0PRESCALER ((1 << CS02) | (1 << CS00)) 	// prescaler 1024; fcpu 16000000 = 16/1024 = 1/64 ticks per microsecond = 64 microseconds per tick
	#define TIMER_US_PT 64								// Microseconds per tick
#else
	#error "This clock speed is not supported"
#endif

#define PULSE_LENGTH_T (PULSE_LENGTH/TIMER_US_PT)	// Pulse length in timer ticks


void beginReceiving();

void refreshCurrentPulseLength();

uint8_t getPassedTime();
uint8_t getPulseAmount();

void setDataHandler(void (*newDataHandler)(uint8_t, uint8_t));

void startReceiving();
uint8_t checkFuzzy(int16_t, int16_t);


void initINT0Interrupt();
void initTimer();
void resetTimer();

#endif		// RFReceiveLib

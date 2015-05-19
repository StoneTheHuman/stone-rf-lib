#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "RFReceiveLib.h"

volatile uint8_t receiving;

volatile uint16_t data = 0;
volatile uint8_t bitsReceived = 0;

volatile uint8_t currentPulseLengthTicks = 0;

void (*dataHandler)(uint8_t, uint8_t);


void beginListening() {
	initINT0Interrupt();
	initTimer();
}

void resumeListening() {
	EIMSK |= (1 << INT0);		// enable INT0
}

void pauseListening() {
	EIMSK &= ~(1 << INT0);		// disable INT0
}

ISR(INTERRUPT_VECTOR) {
	if (COMPARE_VALUE) { // rising edge

		refreshCurrentPulseLength();

		uint8_t pulseAmount = getPulseAmount();

		if (pulseAmount == 9) {
			startReceiving();
		}

		if (receiving) {
			if (pulseAmount == 1 || pulseAmount == 3) {
				data = data << 1;
				if (pulseAmount == 3) {  // LOGICAL ONE
					data |= 1;
				}

				if (++bitsReceived == 16) {
					receiving = 0;
					dataHandler(data >> 8, data);
				}
			}
		}
	} else { // falling edge
		resetTimer();
	}
}

void setDataHandler(void (*newDataHandler)(uint8_t, uint8_t)) {
	dataHandler = newDataHandler;
}

void startReceiving() {
	bitsReceived = 0;
	receiving = 1;
	data = 0;
}

void refreshCurrentPulseLength() {
	currentPulseLengthTicks = TCNT0;
}

uint8_t getPulseAmount() {
	if (checkFuzzy(PULSE_LENGTH_T * 3, currentPulseLengthTicks))
		return 9;
	if (checkFuzzy(PULSE_LENGTH_T * 2, currentPulseLengthTicks))
		return 3;
	if (checkFuzzy(PULSE_LENGTH_T * 1, currentPulseLengthTicks))
		return 1;
	return 0;
}

uint8_t checkFuzzy(int16_t target, int16_t value) {
	return (value > target - MAX_DEV && value < target + MAX_DEV);
}

void initINT0Interrupt() {
	#ifdef MCU_atmega328p
		DDRD &= ~(1 << PD2); 
	#endif
	#ifdef MCU_attiny
		DDRB &= ~(1 << PB2);
	#endif


	EIMSK |= (1 << INT0);		// enable INT0
	EICRA |= (1 << ISC00);		// trigger on voltage change

	sei();
}

void initTimer() {
	TCCR0B = TIM0PRESCALER; 

	sei();
}

void resetTimer() {
	TCNT0 = 0;
}

// only works with 16MHz avr microcontrollers
// data pin needs to be connected to OCR0A 

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "RFSendLib.h"
#include <util/delay.h>


#define HIGH 1;
#define LOW 0;

volatile uint8_t currentState = 0;

volatile Buffer newDataBuffer;
volatile Buffer * dataBuffer;
volatile uint16_t currentSendingData = 0;
volatile uint8_t sentBits = 0;
volatile uint8_t sentIntro = 0;

volatile uint8_t sendingState = 0;

void initTimer() {
	TCCR0B = TIM0PRESCALER;

	TCCR0A |= (1 << COM0A0); // toggle OC0A on Compare Match

	TIMSK0 = (1 << OCIE0A); // on Compare Match interrupt enable

	OCR0A = 0;

	sei();
}

void initLed() {
	DDRB |= (1 << PB4);
}

void initDataBuffer() {
	dataBuffer = &newDataBuffer;

	dataBuffer->size = 8;
	dataBuffer->usedSize = 0;

	dataBuffer->queue = (uint16_t *) malloc(dataBuffer->size * sizeof(uint16_t));
}

void initRFSend() {
	initTimer();

	initLed();

	initDataBuffer();

	DDRB |= 1;
}

void ledOn() {
	PORTB |= (1 << PB4);
}

void ledOff() {
	PORTB &= ~(1 << PB4);
}

void enqueue(uint16_t newData) {
	if (dataBuffer->usedSize >= dataBuffer->size) {
		dataBuffer->size *= 2;
		uint16_t * newQueue = (uint16_t *) malloc(dataBuffer->size * sizeof(uint16_t));
		for (int i=0; i<dataBuffer->usedSize; i++) {
			newQueue[i] = dataBuffer->queue[i];
		}
		free(dataBuffer->queue);
		dataBuffer->queue = newQueue;
	}

	dataBuffer->queue[dataBuffer->usedSize++] = newData;
}

void dequeue() {
	currentSendingData = dataBuffer->queue[0];

	for (int i=1; i<dataBuffer->usedSize; i++) {
		dataBuffer->queue[i-1] = dataBuffer->queue[i];
	}

	dataBuffer->usedSize--;
}

uint8_t queueEmpty() {
	return dataBuffer->usedSize <= 0;
}

void setZeroDelay() {
	OCR0A = (uint8_t) (1 * PULSE_LENGTH_T);
}

void setOneDelay() {
	OCR0A = (uint8_t) (2 * PULSE_LENGTH_T);
}

void setIntroDelay() {
	OCR0A = (uint8_t) (3 * PULSE_LENGTH_T);
}

void setHigh() {
	sendingState = HIGH;
	// PORTB |= 1;
}

void setLow() {
	sendingState = LOW;
	// PORTB &= ~1;
}


ISR(TIM0_COMPA_vect) {
	// PORTB |= 1;
	if (currentState == 0) {
		currentState = HIGH;
		setZeroDelay();
	} else {
		// PORTB &= ~1;
		currentState = LOW;
		if (currentSendingData) {		// sending data atm.
			if (sentIntro) {
				if (currentSendingData & (1 << (15 - sentBits++))) {
					setOneDelay();
				} else {
					setZeroDelay();
				}
				if (sentBits >= 16) {
					sentBits = 0;
					currentSendingData = 0;
					sentIntro = 0;
				}
			} else {
				setIntroDelay();
				sentIntro = 1;
			}
		} else if (sendingState == 1) {
			setOneDelay();
		} else {						// not sending data atm.
			setZeroDelay();
			if (!queueEmpty())
				dequeue();
		}
	}
	TCNT0 = 0;
}

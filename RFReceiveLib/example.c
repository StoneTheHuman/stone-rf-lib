#define F_CPU 1000000

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#include "RFReceiveLib.h"

#define DEVICE_ADDRESS 37	// This is the address that should be unique for your device. You can have up to 256 devices. (0-255)

#define LED PB3

void initLed();
void ledOn();
void ledOff();

static void dataHandler(uint8_t address, uint8_t command) {
	if (address == DEVICE_ADDRESS && command == 0) {
		ledOff();
	} 
	if (address == DEVICE_ADDRESS && command == 255) {
		ledOn();
	} 
}

int main() {
	initLed();

	setDataHandler(&dataHandler);

	beginListening();

	for(;;);
}

void initLed() {
	DDRB |= (1 << LED);
}

void ledOn() {
	PORTB |= (1 << LED);
}

void ledOff() {
	PORTB &= ~(1 << LED);
}

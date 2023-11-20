#include "system_sam3x.h"
#include "at91sam3x8.h"

#include <stdio.h>

// Databus functions

typedef struct databus_s {
	int dir;
	int databus_pins[];
} databus;

databus display = {
	13,
	{1, 3, 4, 5, 6, 7, 8, 9}
};

void SetDir(databus db, int dir) {
	*AT91C_PIOC_SODR = (dir ? 1 : 0 << db.dir);
}

void SetBusAsOutput(databus db, int value) {
	unsigned int* address = value ? AT91C_PIOD_OER : AT91C_PIOD_ODR;
	for (int i = 0; i < 8; i++) {
		*address = (1 << db.databus_pins[i]);
	}
}

int pollPanel() {
	int value = 0;
	// Clear OE KEY BUS (Active Low)
	// Make all Column pin as high output
	*AT91C_PIOC_OER = (1 << 7) | (1 << 8) | (1 << 9);
	*AT91C_PIOC_SODR = (1 << 7) | (1 << 8) | (1 << 9);
	// Loop Column
	for(int col = 0; col < 3; col++) {
		// Clear one column at the time
		*AT91C_PIOC_CODR = (1 << (col + 7));
		// Loop Row
		for(int row = 0; row < 4; row++) {
			// Read row and check if bit is zero             
			if((*AT91C_PIOC_PDSR & (1 << (row + 3))) == 0) {
				value = row * 3 + col + 1;
       			printf("%d\n", value);
				return value;
			}
		}
		// Set the column again
		*AT91C_PIOC_SODR = (1 << (col + 7));
	}
	return value;
}

void KeypadInit() {
    // Configure COL pins as output
    *AT91C_PIOC_PER = (1 << 7) | (1 << 8) | (1 << 9);
    *AT91C_PIOC_OER = (1 << 7) | (1 << 8) | (1 << 9);
    *AT91C_PIOC_PPUDR = (1 << 7) | (1 << 8) | (1 << 9);

    // Configure ROW pins as input (pin 34 is faulty and can't read input, all the pins have been shifted by one)
    *AT91C_PIOC_PER = (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
    *AT91C_PIOC_ODR = (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);
}

void Delay(int value) {
	for (int i = 0; i < value; i++)
		asm("nop");
}

unsigned char Read_Status_Display(void) {
	unsigned char temp;

	// make databus as input
	SetBusAsOutput(display, 0);
	// Set dir as input (74chip, 1 = input)
	SetDir(display, 1);
	// Clear/enable output (74chip 0 = enable)
	// Set C/D
	// Clear chip select display
	// Clear read display
	// Make a Delay
	// Read data bus and save it in temp
	// Set chip select display
	// Set read display
	// Disable output (74chip)
	SetBusAsOutput(display, 1);
	// Set dir as output (74chip)
	SetDir(display, 0);
	// Return (Temp)
} 

void main(void) {
    SystemInit();

    *AT91C_PMC_PCER = (1 << 13);
    *AT91C_PMC_PCER = (1 << 12);


	// Configure OEBA pin as output and set it to high
	*AT91C_PIOD_PER = (1 << 2);
	*AT91C_PIOD_OER = (1 << 2);

    while(1){
		pollPanel();
    //   ReadButton(&buttonVal);
    //   SetLed(buttonVal);
    }
}

/**
 * 	FOR THE DISPLAY :
 * pin 34 moved to pin 33
 * 
 */
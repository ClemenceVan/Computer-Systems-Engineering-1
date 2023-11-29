#include "system_sam3x.h"
#include "at91sam3x8.h"

#include <stdio.h>
#include <stdarg.h>

// Databus functions

typedef struct databus_s {
	int dir[2];
	int databus_pins[8];
} databus;

databus display = {
	{13, 12},
	{2, 3, 4, 5, 6, 7, 8, 9}
};

void SetDir(databus db, int dir) {
	if (dir) {
		*AT91C_PIOC_SODR = (1 << db.dir[0]);
		// *AT91C_PIOC_SODR = (1 << db.dir[1]);
	} else {
		*AT91C_PIOC_CODR = (1 << db.dir[0]);
		// *AT91C_PIOC_CODR = (1 << db.dir[1]);
	}
}

void SetBusAsOutput(databus db, int value) {
	for (int i = 0; i < 8; i++) {
		if (value)
			*AT91C_PIOC_OER = (1 << db.databus_pins[i]);
		else
			*AT91C_PIOC_ODR = (1 << db.databus_pins[i]);
	}
}

unsigned char ReadDatabus(databus db) {
	unsigned char temp = 0b00000000;
	// temp |=  ((*AT91C_PIOC_PDSR & (1 << 2)) >> 2) << 0;
	// temp |=  ((*AT91C_PIOC_PDSR & (1 << 3)) >> 3) << 1;
	// temp |=  ((*AT91C_PIOC_PDSR & (1 << 4)) >> 4) << 2;
	// temp |=  ((*AT91C_PIOC_PDSR & (1 << 5)) >> 5) << 3;
	// temp |=  ((*AT91C_PIOC_PDSR & (1 << 6)) >> 6) << 4;
	// temp |=  ((*AT91C_PIOC_PDSR & (1 << 7)) >> 7) << 5;
	// temp |=  ((*AT91C_PIOC_PDSR & (1 << 8)) >> 8) << 6;
	// temp |=  ((*AT91C_PIOC_PDSR & (1 << 9)) >> 9) << 7;
	temp = (*AT91C_PIOC_PDSR >> 2) & 0xFF;

	return temp;
}

void SetDatabus(databus db, unsigned char value) {
	// for (int i = 0; i < 8; i++) {
	// 	if ((value >> (7 - i)) & 1) {
	// 		*AT91C_PIOC_SODR = (1 << db.databus_pins[i]);
	// 	} else {
	// 		*AT91C_PIOC_CODR = (1 << db.databus_pins[i]);
	// 		// *AT91C_PIOC_ODR = (1 << db.databus_pins[i]);
    // 	}
	// }
	// *AT91C_PIOC_SODR = value << 2;
	*AT91C_PIOC_CODR |= (0xFF << 2);
	*AT91C_PIOC_SODR |= (value << 2);
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

    // Configure ROW pins as input
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
	// Set dir as input (74chip, 1 = input) // Clear/enable output (74chip 0 = enable) // check about pin 48
	SetDir(display, 1);
	// Set C/D
	*AT91C_PIOC_SODR = 1<<14;
	*AT91C_PIOC_SODR = 1<<17;
	// Clear chip select display
	*AT91C_PIOC_CODR = 1<<15; // was at 3
	// Clear read display
	*AT91C_PIOC_CODR = 1<<16;

	// *AT91C_PIOC_SODR = 1<<1;
	// Make a Delay
	Delay(100);
	// Read data bus and save it in temp
	temp = ReadDatabus(display);
	// Set chip select display
	*AT91C_PIOC_SODR =  1<<15; // was at 3
	// Set read display
	*AT91C_PIOC_SODR = 1<<16;
	// Disable output (74chip)
	SetBusAsOutput(display, 1);
	// Set dir as output (74chip)
	SetDir(display, 0);
	// Return (Temp)
	printf("temp: %x\n", temp);
	return temp;
}

void Write_Command_2_Display(unsigned char Command) {
	// Wait until Read_Status_Display returns an OK
	while ((Read_Status_Display() & 3) != 3) asm("nop");
	// Clear databus
	SetDatabus(display, 0);
	// Set Command to databus
	SetDatabus(display, Command);
	printf("Command: %x\n", Command);
	// Set dir as output (74chip)
	SetDir(display, 0);
	// Enable output (74chip)
	// *AT91C_PIOC_CODR = 1<<12;
	// Set databus as output
	SetBusAsOutput(display, 1);
	// Set C/D signal High (1 = Command)
	*AT91C_PIOC_SODR = 1<<14;
	// Clear chip select display
	*AT91C_PIOC_CODR = 1<<16;
	// Clear write display
	*AT91C_PIOC_CODR = 1<<17;
	// Make a Delay
	Delay(100);
	// Set chip enable display
	*AT91C_PIOC_SODR = 1<<16;
	// Set write display
	*AT91C_PIOC_SODR = 1<<17;
	// Disable output (74chip)
	// *AT91C_PIOC_SODR = 1<<12;
	// Make databus as input 
	SetDir(display, 1);
	SetBusAsOutput(display, 0);
}

void Write_Data_2_Display(unsigned char Data) {
	// Wait until Read_Status_Display returns an OK
	while ((Read_Status_Display() & 3) != 3) asm("nop");
	// Clear databus
	SetDatabus(display, 0);
	// Set Data to databus
	SetDatabus(display, Data);
	// Set dir as output (74chip)
	SetDir(display, 0);
	// Enable output (74chip)
	// *AT91C_PIOC_CODR = 1<<12;
	// *AT91C_PIOC_CODR = 1<<13;
	// // Set databus as output
	SetBusAsOutput(display, 1);
	// Clear C/D signal High (0 = Data)
	*AT91C_PIOC_CODR = 1<<14;
	// Clear chip select display
	*AT91C_PIOC_CODR = 1<<16;
	// Clear write display
	*AT91C_PIOC_CODR = 1<<17;
	// Make a Delay
	Delay(100);
	// Set chip enable display
	*AT91C_PIOC_SODR = 1<<16;
	// Set write display
	*AT91C_PIOC_SODR = 1<<17;
	// Disable output (74chip)
	// *AT91C_PIOC_SODR = 1<<12;
	SetDir(display, 1);
	// Make databus as input 
	SetBusAsOutput(display, 0);
}

void Init_Display(void) {
	// Clear Reset display
	*AT91C_PIOD_CODR = 1<<0;
	// Make a Delay
	Delay(100);
	// Set Reset display
	*AT91C_PIOD_SODR = 1<<0;
	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x40);//Set text home address
	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x40);
	Write_Command_2_Display(0x42); //Set graphic home address
	Write_Data_2_Display(0x1e);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x41); // Set text area
	Write_Data_2_Display(0x1e);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x43); // Set graphic area
	Write_Command_2_Display(0x80); // text mode
	Write_Command_2_Display(0x94); // Text on graphic off
}

void DisplayPrintf(char* string, ...) {
	char buffer[256];
	va_list args;
	va_start(args, string);
	vsprintf(buffer, string, args);
	va_end(args);
	for (int i = 0; buffer[i] != '\0'; i++) {
		Write_Data_2_Display(buffer[i] - 0x20);
		Write_Command_2_Display(0xC0);
	}
}

void main(void) {
    SystemInit();
	// KeypadInit();

    *AT91C_PMC_PCER = (1 << 13);
    *AT91C_PMC_PCER = (1 << 12);


	// Configure OEBA pin as output and set it to high FOR KEYPAD
	// *AT91C_PIOD_PER = (1 << 2);
	// *AT91C_PIOD_OER = (1 << 2);


	*AT91C_PIOC_PER = 1<<2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1<<8 | 1<<9 | 1<<12 | 1<<13 | 1<<14 | 1<<15 | 1<<16 | 1<<17;
	*AT91C_PIOD_PER = 1<<0;
	
	*AT91C_PIOC_ODR = 1<<2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1<<8 | 1<<9;
	*AT91C_PIOC_OER = 1<<12 | 1<<13 | 1<<14 | 1<<15 | 1<<16 | 1<<17;
	*AT91C_PIOD_OER = 1<<0;

	*AT91C_PIOD_SODR = 1<<12;

	Init_Display();

     while(1) {
	// 	// DisplayPrintf("%d", pollPanel());
	 	DisplayPrintf("A");
     }
}

/**
 * 	FOR THE DISPLAY :
 * pin 34 moved to pin 33
 * 
 */
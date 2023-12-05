#include "display.h"

databus display = {
	13, 
	12,
	{2, 3, 4, 5, 6, 7, 8, 9}
};

unsigned char Read_Status_Display(void) {
	unsigned char temp;
	SetBusAsOutput(display, 0);
	SetDir(display, 1);
	*AT91C_PIOC_SODR = CD;
	*AT91C_PIOC_SODR = WR;
	*AT91C_PIOC_CODR = CE;
	*AT91C_PIOC_CODR = RD;

	Delay(10);
	temp = ReadDatabus(display);
	
	*AT91C_PIOC_SODR = CE;
	*AT91C_PIOC_SODR = RD;
	SetBusAsOutput(display, 1);
	SetDir(display, 0);
	return temp;
}

void Write_Command_2_Display(unsigned char Command) {
	while ((Read_Status_Display() & 3) != 3) asm("nop");
	SetDatabus(display, 0);
	SetDatabus(display, Command);
	SetDir(display, 0);
	SetBusAsOutput(display, 1);
	*AT91C_PIOC_SODR = CD;
	*AT91C_PIOC_CODR = CE;
	*AT91C_PIOC_CODR = WR;

	Delay(10);
	
	*AT91C_PIOC_SODR = CE;
	*AT91C_PIOC_SODR = WR;
	SetDatabus(display, 0);
	SetBusAsOutput(display, 0);
}

void Write_Data_2_Display(unsigned char Data) {
	while ((Read_Status_Display() & 3) != 3) asm("nop");
	SetDatabus(display, 0);
	SetDatabus(display, Data);
	SetDir(display, 0);
	SetBusAsOutput(display, 1);
	*AT91C_PIOC_CODR = CD;
	*AT91C_PIOC_CODR = CE;
	*AT91C_PIOC_CODR = WR;

	Delay(10);
	
	*AT91C_PIOC_SODR = CE;
	*AT91C_PIOC_SODR = WR;
	SetDatabus(display, 0);
	SetBusAsOutput(display, 0);
}

void Init_Display(void) {
	// Reset display sequence
	*AT91C_PIOD_CODR = 1 << 0;
	Delay(10);
	*AT91C_PIOD_SODR = 1 << 0;

	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x40);//Set text home address
	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x40);
	Write_Command_2_Display(0x42); //Set graphic home address
	Write_Data_2_Display(0x28);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x41); // Set text area
	Write_Data_2_Display(0x28);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x43); // Set graphic area
	Write_Command_2_Display(0x80); // text mode
	Write_Command_2_Display(0x94); // Text on graphic off
	Write_Data_2_Display(0x01);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0xD0); // Text blink off
	clearDisplay();
}

void displayPrintf(char* string, ...) {
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

void clearDisplay(void) {
	for (int i = 0; i < 1024; i++) {
		Write_Data_2_Display(0x00);
		Write_Command_2_Display(0xC0);
	}
	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x24);
}

void DisplayInit(void) {
	*AT91C_PIOC_PER = 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1 << 8 | 1 << 9 | 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15 | 1 << 16 | 1 << 17;
	*AT91C_PIOD_PER = 1 << 0;
	
	*AT91C_PIOC_ODR = 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1 << 8 | 1 << 9;
	*AT91C_PIOC_OER = 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15 | 1 << 16 | 1 << 17;
	*AT91C_PIOD_OER = 1 << 0;

	*AT91C_PIOD_SODR = 1 << 12;
}
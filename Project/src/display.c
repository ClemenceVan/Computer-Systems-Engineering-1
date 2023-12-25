#include "Display.h"
#include "Timer.h"

unsigned char Read_Status_Display(void) {
	unsigned char tmp;
	SetBusAsOutput(Display.db, 0);
	SetDir(Display.db, 1);
	*AT91C_PIOC_SODR = CD;
	*AT91C_PIOC_SODR = WR;
	*AT91C_PIOC_CODR = CE;
	*AT91C_PIOC_CODR = RD;

	Delay(10);
	tmp = ReadDatabus(Display.db);
	
	*AT91C_PIOC_SODR = CE;
	*AT91C_PIOC_SODR = RD;
	SetBusAsOutput(Display.db, 1);
	SetDir(Display.db, 0);
	return tmp;
}

void Write_Command_2_Display(unsigned char Command) {
	*AT91C_PIOC_IDR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	while ((Read_Status_Display() & 3) != 3) asm("nop");
	SetDatabus(Display.db, 0);
	SetDatabus(Display.db, Command);
	SetDir(Display.db, 0);
	SetBusAsOutput(Display.db, 1);
	*AT91C_PIOC_SODR = CD;
	*AT91C_PIOC_CODR = CE;
	*AT91C_PIOC_CODR = WR;

	Delay(10);
	
	*AT91C_PIOC_SODR = CE;
	*AT91C_PIOC_SODR = WR;
	SetDatabus(Display.db, 0);
	SetBusAsOutput(Display.db, 0);
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	if (!Timer.Flags.keypad)
		*AT91C_PIOC_IER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
}

void Write_Data_2_Display(unsigned char Data) {
	*AT91C_PIOC_IDR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	while ((Read_Status_Display() & 3) != 3) asm("nop");
	SetDatabus(Display.db, 0);
	SetDatabus(Display.db, Data);
	SetDir(Display.db, 0);
	SetBusAsOutput(Display.db, 1);
	*AT91C_PIOC_CODR = CD;
	*AT91C_PIOC_CODR = CE;
	*AT91C_PIOC_CODR = WR;

	Delay(10);
	
	*AT91C_PIOC_SODR = CE;
	*AT91C_PIOC_SODR = WR;
	SetDatabus(Display.db, 0);
	SetBusAsOutput(Display.db, 0);
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_ISR;
	if (!Timer.Flags.keypad) // COULD BE THE ISSUE
		*AT91C_PIOC_IER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
}

void display_enable(void) {
	// Reset Display.db sequence
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
	Display.clear();
}

int displayCursor(int x, int y) { 
	if (x < 0 || x > DISPLAY_WIDTH || y < 0 || y > DISPLAY_HEIGHT)
		return -1;
	int pos = y * DISPLAY_WIDTH + x;
	Write_Data_2_Display((unsigned char)pos);
	Write_Data_2_Display((unsigned char)(pos >> 8));
	Write_Command_2_Display(0x24);
	return 0;
}

void displayPrintfAt(int pos[2], char* string, ...) {
	if (displayCursor(pos[0], pos[1]) == -1)
		return;
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
	displayCursor(0, 0);
    for (int i = 0; i < 1024; i++) {
        Write_Data_2_Display(0x00);
        Write_Data_2_Display(0x00);
        Write_Command_2_Display(0xC0);
    }
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x24);
}

void DisplayInit(void) {
	*AT91C_PIOC_PER = DATABUS_MASK | 1 << Display.db.dir | 1 << Display.db.OE | DISPLAYCONTROL_MASK;
	*AT91C_PIOD_PER = 1 << 0;
	
	*AT91C_PIOC_ODR = DATABUS_MASK;
	*AT91C_PIOC_OER = 1 << Display.db.dir | 1 << Display.db.OE | DISPLAYCONTROL_MASK;
	*AT91C_PIOD_OER = 1 << 0;

	*AT91C_PIOD_SODR = 1 << Display.db.OE;
}

display Display = {
	.printfAt = displayPrintfAt,
	.clear = clearDisplay,
	.init = DisplayInit,
	.enable = display_enable,
	.db = {
		.dir = 13,
		.OE = 12,
		.data_pins = {2, 3, 4, 5, 6, 7, 8, 9}
	}
};
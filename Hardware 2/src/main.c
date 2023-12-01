#include "include.h"
#include "databus.h"
#include "keypad.h"
#include "display.h"

void Delay(int value) {
	for (int i = 0; i < value; i++)
		asm("nop");
}

void init(void) {
    SystemInit();
	// Enable PIO
	*AT91C_PMC_PCER = (1 << 13);
	*AT91C_PMC_PCER = (1 << 12);
	
	// Enable peripherals pins
	KeypadInit();
	DisplayInit();


	Init_Display();
}

void main(void) {
	init();

	while(1) {
		clearDisplay();
		displayPrintf("%d", pollPanel());
		Delay(1000000);
     }
}
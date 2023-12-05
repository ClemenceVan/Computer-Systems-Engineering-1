#include "include.h"
#include "databus.h"
#include "keypad.h"
#include "display.h"
#include "timer.h"
#include "temp.h"

void Delay(int value) {
	for (int i = 0; i < value; i++)
		asm("nop");
}

int tempReady = 0;

void TC0_Handler(void) {
	tempReady = 1;
	*AT91C_TC0_IDR=1<<6;
	*AT91C_TC0_SR;
}

void handle() {
	if(tempReady) {
		tempReady = 0;
		float temp = GetTemp();
		clearDisplay();
		displayPrintf("temp: %f", temp);
		TempStartReset();
		TempStart();
	}
}

void init(void) {
    SystemInit();
	// Enable PIO
	*AT91C_PMC_PCER = (1 << 13);
	*AT91C_PMC_PCER = (1 << 12);
	
	// Enable peripherals pins
	// KeypadInit();
	DisplayInit();
    TempSensorInit();
	InitTimer(10, handle);

	TempStart();
	Init_Display();
	
}

void main(void) {
	init();

	while(1) {
		// Delay(1000000);
    }
}
#include "include.h"
#include "databus.h"
#include "keypad.h"
#include "display.h"
#include "timer.h"
#include "temp.h"
#include "lightsensor.h"
#include "servo.h"
#include "calendar.h"

void Delay(int value) {
	for (int i = 0; i < value; i++)
		asm("nop");
}

void TC0_Handler(void) {
	Timer.Flags.temp = 1;
	Temperature.disable();
}

void ADC_Handler(void) {
	Timer.Flags.light = 1;
	Light.disable();
}


void handle() {
	// Update time
	static int toSec = 0;
	toSec += 10;
	if (toSec >= 1000) {
		toSec = 0;
		Calendar.now += 1;
	}


		Display.printfAt((int[2]){(DISPLAY_WIDTH/2)-6, DISPLAY_HEIGHT/2}, " SMART HOME ");
        Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[1]Calendar ");
        Display.printfAt((int[2]){13, DISPLAY_HEIGHT}, "[2]Recordings ");
        Display.printfAt((int[2]){29, DISPLAY_HEIGHT}, "[3]Settings ");
	
	switch(Keypad.poll()){
		
		case 0:
		break;

		case 1:
		Display.clear();
		Display.printfAt((int[2]){0, 0}, "CALENDAR");
		Display.printfAt((int[2]){0, 1}, "dd/mm/yyyy");
		break;

		case 2:
		Display.clear();
		Display.printfAt((int[2]){0, 0}, "RECORDINGS");
		Display.printfAt((int[2]){0, 1}, "Latest Recorded temp:");
		break;

		case 3:
		Display.clear();
		Display.printfAt((int[2]){0, 0}, "SETTINGS");
		Display.printfAt((int[2]){0, 1}, "Temperature limits:");
		break;

		default: 
		Display.clear();
		
	}
	Display.printfAt((int[2]){DISPLAY_WIDTH - 19, 0}, Calendar.toString(Calendar.getNow()));
}

void init(void) {
    SystemInit();
	// Enable PIO
	*AT91C_PMC_PCER = (1 << 13);
	*AT91C_PMC_PCER = (1 << 12);
	
	// Enable peripherals pins
	Keypad.init();
	Servo.init();
	Display.init();
	Temperature.init();
	Light.init();

	Timer.init(10, handle);
	Temperature.enable();
	Light.enable();
	Display.enable();
	Display.clear();
}

void PIOD_Handler(void) {
	switch (*AT91C_PIOD_ISR) {
		case 1 << 1:
			Servo.steer(10);
			break;
		case 1 << 9:
			Servo.steer(20);
			break;
	}
}

void main(void) {
	init();


	// For buttons, to be cleaned up
    *AT91C_PMC_PCER = (1 << 14);
	*AT91C_PIOD_PER = 1 << 1 | 1 << 9;

	*AT91C_PIOD_ODR = 1 << 1 | 1 << 9;
    *AT91C_PIOD_PPUER = 1 << 1 | 1 << 9;

    *AT91C_PIOD_ISR;
    NVIC_ClearPendingIRQ(PIOD_IRQn);
    NVIC_SetPriority(PIOD_IRQn, 0);
    NVIC_EnableIRQ(PIOD_IRQn);
    
    *AT91C_PIOD_IER = 1 << 1 | 1 << 9;

	while(1) {
    }
}
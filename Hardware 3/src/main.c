#include "include.h"
#include "databus.h"
#include "keypad.h"
#include "display.h"
#include "timer.h"
#include "temp.h"
#include "lightsensor.h"
#include "servo.h"

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
	Display.printfAt((int[2]){8, 15}, "  ", Keypad.poll());
	Display.printfAt((int[2]){0, 15}, "Keypad: %d", Keypad.poll());
	if(Timer.Flags.temp) {
		Timer.Flags.temp = 0;
		float temp = Temperature.get();
		Temperature.enable();
		Display.printfAt((int[2]){0, 0}, "temp: %f", temp);
	}
	if (Timer.Flags.light) {
		Timer.Flags.light = 0;
		Display.printfAt((int[2]){DISPLAY_WIDTH - 15, 0}, "light: %f", Light.get());
		Light.enable();
	}
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
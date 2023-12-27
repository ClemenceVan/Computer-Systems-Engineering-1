#include "./include.h"
#include "scenes/scenes.h"
#include "peripherals/databus.h"
#include "peripherals/keypad.h"
#include "peripherals/display.h"
#include "peripherals/timer.h"
#include "peripherals/temp.h"
#include "peripherals/lightsensor.h"
#include "peripherals/servo.h"
#include "calendar.h"

#define IDLE 0
#define READY 1
#define AUTO_RECORDING 2
#define AUTO_READY 3

void Delay(int value) {
	for (int i = 0; i < value; i++)
		asm("nop");
}

void TC0_Handler(void) {
	if (Timer.Flags.temp == AUTO_RECORDING) Timer.Flags.temp = AUTO_READY;
	else Timer.Flags.temp = READY;
	Temperature.disable();
}

void ADC_Handler(void) {
	Timer.Flags.light = 1;
	Light.disable();
}

int calendarScene() {
	Display.printfAt((int[2]){0, 0}, "CALENDAR");
	Display.printfAt((int[2]){0, 1}, "dd/mm/yyyy");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back");
	while (1) {
		if (Keypad.key == 11) return 0;
	}
}

int switchScreen(int screen) {
	Keypad.key = 0;
	if (screen < 0 || screen >= sizeof(screens)/sizeof(screens[0])) return -1;
	currentScreen = screen;
	Display.clear();
	return screens[screen]();
}

void handle() {
	// Watch for temperature recording
	// if (Temperature.initialized && elapsed % 60000 == 0) {
	// 	Timer.Flags.temp = AUTO_RECORDING;
	// 	Temperature.start();
	// 	elapsed = 0;
	// }

	// switch (Timer.Flags.temp) {
	// 	case IDLE:
	// 		break;
		
	// 	case READY:
	// 		// TBD
	// 		if (Temperature.enabled) Temperature.start();
	// 		break;
	// 	case AUTO_READY:
	// 		Calendar.addTemperture(Temperature.get()); // to be tested
	// 		if (Temperature.enabled) Temperature.start();
	// 		break;
	// 	default:
	// 		break;
	// }

	// if (Timer.Flags.temp != IDLE) {
	// 	if (Timer.Flags.temp == READY) {
	// 		// TBD
	// 	}
	// 	else if (Timer.Flags.temp == AUTO_READY) {
	// 		Display.printfAt((int[2]){0, 1}, "Latest Recorded temp: %.2f", Temperature.get());
	// 	}
	// 	Timer.Flags.temp = IDLE;
	// 	Temperature.disable();
	// }
	// Display.printfAt((int[2]){DISPLAY_WIDTH - 19, 0}, Calendar.toString(Calendar.getNow()));
}

void init(void) {
    SystemInit();
	// Enable PIO
	*AT91C_PMC_PCER = (1 << 13);
	*AT91C_PMC_PCER = (1 << 12);
	
	// Enable peripherals pins
	Keypad.init();
	// Servo.init();
	Display.init();
	// Temperature.init();
	// Light.init();

	// Temperature.enable();
	Timer.init(10, handle);
	// Light.enable();
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

void PIOC_Handler(void) {
	*AT91C_PIOC_IDR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	Keypad.key = Keypad.poll();
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_IER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	// Keypad.key = t ? t : Keypad.key;
	// Timer.Flags.keypad = 1;
}



void main(void) {
	init();

	Display.printfAt((int[2]){(DISPLAY_WIDTH/2)-6, DISPLAY_HEIGHT/2}, " SMART HOME ");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[1]Calendar ");
	Display.printfAt((int[2]){13, DISPLAY_HEIGHT}, "[2]Recordings ");
	Display.printfAt((int[2]){29, DISPLAY_HEIGHT}, "[3]Settings ");

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

	Calendar.setDateTime((Date) {
			.day = 21,
			.month = 12,
			.year = 2023,
			.time = (Time) {
				.hour = 13,
				.minute = 17,
				.second = 0
			}
		});
	
	// keypad interrupts, should be moved to keypad.c
    *AT91C_PIOC_PPUDR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    *AT91C_PIOC_IER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_AIMER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_ESR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_FELLSR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	NVIC_ClearPendingIRQ(PIOC_IRQn);
	NVIC_SetPriority(PIOC_IRQn, 0);
	NVIC_EnableIRQ(PIOC_IRQn);
	*AT91C_PIOC_ISR;

	
	while(1) {
		currentScreen = switchScreen(currentScreen);//currentScreen);
    }
}
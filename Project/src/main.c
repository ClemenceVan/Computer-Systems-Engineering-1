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

#include <string.h>

#define IDLE 0
#define READING 1
#define READY 2

master Master = {
	.flags = {
		.temp = IDLE,
		.light = 0,
		.keypad = 0,
		.alert = 0
	},
	.params = {
		.tempLimits = {20, 25},
		.fastMode = 0
	}
};

void Delay(int value) {
	for (int i = 0; i < value; i++)
		asm("nop");
}

void TC0_Handler(void) {
	Temperature.stop();
	Master.flags.temp = READY;
}

void ADC_Handler(void) {
	Master.flags.light = 1;
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
	static unsigned long elapsed = 0;
	static int recordTemp = 0;
	if (Master.flags.alert) {
		if (*AT91C_PIOB_ODSR & (1 << 27))
			*AT91C_PIOB_CODR = 1 << 27;
		else
			*AT91C_PIOB_SODR = 1 << 27;
	} else if (elapsed % 100 == 0) {
		recordTemp = 1;
	}
	elapsed += 10;
	switch (Master.flags.temp) {
		case IDLE:
			break;
		
		case READY:
			float temp = Temperature.get();
			if (!Master.flags.alert && (temp < Master.params.tempLimits[0] || temp > Master.params.tempLimits[1])) {
				printf("%d, %d\n", Master.params.tempLimits[0], Master.params.tempLimits[1]);
				printf("%d, %d\n", temp < Master.params.tempLimits[0], temp > Master.params.tempLimits[1]);
				Master.flags.alert = malloc(sizeof(char) * 24);
				strcpy(Master.flags.alert, temp > Master.params.tempLimits[0] ? "Temperature too high!" : "Temperature too low!");
			}
			if (recordTemp || Master.params.fastMode) {
				Calendar.addTemperature(temp); // to be tested
				recordTemp = 0;
			}
			Temperature.start();
			Master.flags.temp = READING;
			break;
		case READING:
			break;
		default:
			break;
	}

	// if (Master.flags.temp != IDLE) {
	// 	if (Master.flags.temp == READY) {
	// 		// TBD
	// 	}
	// 	else if (Master.flags.temp == AUTO_READY) {
	// 		Display.printfAt((int[2]){0, 1}, "Latest Recorded temp: %.2f", Temperature.get());
	// 	}
	// 	Master.flags.temp = IDLE;
	// 	Temperature.disable();
	// }
	// Display.printfAt((int[2]){DISPLAY_WIDTH - 19, 0}, Calendar.toString(Calendar.getNow()));
}

void init(void) {
    SystemInit();
	// Enable PIO
	*AT91C_PMC_PCER = (1 << 13);
	*AT91C_PMC_PCER = (1 << 12);
	*AT91C_PIOB_PER = 1 << 27;
	*AT91C_PIOB_OER = 1 << 27;

	// Enable peripherals pins
	Keypad.init();
	// Servo.init();
	Display.init();
	Temperature.init();
	// Light.init();

	Temperature.enable();
	Temperature.start();
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
	// Master.flags.keypad = 1;
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

	/* Temp TEST mode */
	// add 3 previous days of temperatures for every 30 minutes
	Calendar.now = Calendar.now - 86400 * 3;
	for (int i = 0; i < 72; i++) {
		Calendar.addTemperature(20 + (rand() % 10) - 5);
		Calendar.now += 1800;
	}
	Calendar.getRecordings(Calendar.getEndOfDay() - 86400 * 2, Calendar.getEndOfDay());
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
	/*                */
	while(1) {
		currentScreen = switchScreen(currentScreen);//currentScreen);
    }
}
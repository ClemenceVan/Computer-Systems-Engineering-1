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

int switchScreen(int screen) {
	Keypad.key = 0;
	if (screen < 0 || screen >= sizeof(screens)/sizeof(screens[0])) return -1;
	currentScreen = screen;
	Display.clear();
	return screens[screen]();
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
	Servo.init();
	Display.init();
	Temperature.init();
	Light.init();

	Temperature.start();
	Timer.init(10, handle);
	Light.enable();
	Display.enable();
	Display.clear();

}

void main(void) {
	init();

	Calendar.setDateTime((Date) {
			.day = 11,
			.month = 1,
			.year = 2024,
			.time = (Time) {
				.hour = 12,
				.minute = 0,
				.second = 0
			}
		});
	
	/* Temp TEST mode */
		// // add 3 previous days of temperatures for every 30 minutes
		// Calendar.now = Calendar.now - 86400 * 3;
		// for (int i = 0; i < 72; i++) {
		// 	Calendar.addTemperature(20 + (rand() % 10) - 5);
		// 	Calendar.now += 1800;
		// }
		// Calendar.getRecordings(Calendar.getEndOfDay() - 86400 * 2, Calendar.getEndOfDay());
		// Calendar.setDateTime((Date) {
		// 		.day = 21,
		// 		.month = 12,
		// 		.year = 2023,
		// 		.time = (Time) {
		// 			.hour = 13,
		// 			.minute = 17,
		// 			.second = 0
		// 		}
		// 	});
	/*                */

	while(1) currentScreen = switchScreen(currentScreen);
}
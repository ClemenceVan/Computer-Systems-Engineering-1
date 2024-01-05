#include "./include.h"
#include "peripherals/databus.h"
#include "peripherals/keypad.h"
#include "peripherals/display.h"
#include "peripherals/timer.h"
#include "peripherals/temp.h"
#include "peripherals/lightsensor.h"
#include "peripherals/servo.h"
#include "calendar.h"

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
}

void TC0_Handler(void) {
	Temperature.stop();
	Master.flags.temp = READY;
}

void ADC_Handler(void) {
	Light.disable();
	Master.flags.light = READY;
}

void handle() {
	static int elapsed = 0; // time elapsed since last temperature record
	if (Master.flags.alert) { // amber led blinking
		if (*AT91C_PIOB_ODSR & (1 << 27))
			*AT91C_PIOB_CODR = 1 << 27;
		else
			*AT91C_PIOB_SODR = 1 << 27;
	}
	if (Master.flags.temp == READY) {
		float temp = Temperature.get();
		if (!Master.flags.alert && (temp < Master.params.tempLimits[0] || temp > Master.params.tempLimits[1])) {
			Master.flags.alert = malloc(sizeof(char) * 24); // freed in menu scene
			strcpy(Master.flags.alert, temp > Master.params.tempLimits[0] ? "Temperature too high!" : "Temperature too low!");
		}
		if (elapsed > 100 || Master.params.fastMode) { // /!\ NEED TO MAKE SURE THIS CORRESPONDS TO 30 MINUTES
			Calendar.addTemperature(temp);
			elapsed = 0;
		}
		Temperature.start();
		Master.flags.temp = READING;
	}
	if (Master.flags.light) {
		float *light = Light.get();
		int angle = (int)(light[0] - light[1]);
		if (angle < 0) angle = -angle;
		angle = angle % 180;
		if (light[0] - light[1] < 0) angle = 180 - angle;
		Light.enable();
		Master.flags.light = READING;
	}
	elapsed += 10;
}
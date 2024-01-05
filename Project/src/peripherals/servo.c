#include "./servo.h"

void setPos(int angle) { // page 991 in datasheet
	if (angle < 0 || angle > 180)
		angle = 0;
	Servo.position = angle;
	*AT91C_PWMC_CH1_CDTYR = 1300 + 27.2 * angle;
}

void steer(int angle) {
	// add angle to current position
	if (angle < 0 || angle > 180)
		return;
	setPos(Servo.position + angle);
}

void ServoInit(void)
{
	*AT91C_PMC_PCER1 = (1<<4);//init PWM
	*AT91C_PIOB_PDR = (1<<17); // give control to peripheral PWM

	*AT91C_PIOB_ABMR = (1<<17);
	*AT91C_PWMC_ENA = (1<<1); //enable PWM

	*AT91C_PWMC_CH1_CMR = 0x5; //set prescaler (clk/32)

	*AT91C_PWMC_CH1_CPRDR = 52500; //set period
  	*AT91C_PWMC_CH1_CDTYR = 1300;
	setPos(0);
}

servo Servo = {
	.setPos = setPos,
	.steer = steer,
	.init = ServoInit,
	.position = 0
};
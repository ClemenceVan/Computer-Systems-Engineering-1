#include "./lightsensor.h"
#include "./servo.h"

void LightSensorInit() {
	*AT91C_PMC_PCER1 = (1<<5); //Enable PMC for ADC
	*AT91C_PIOA_PDR = 1 << 4 | 1 << 5; //Disable PIO control over pins
	*AT91C_ADCC_MR = (0xFFFF<<8); //Set the prescaler (ADCClock = MCK / ( (PRESCAL+1) * 2 )) (MCK = 4MHz)   PRESCAL= MCK/2*ADCClock-1 = 4MHz/2
	*AT91C_ADCC_CHER = 0x6;// 1<<2|1<<3; // Enable channel 1 (A5 & A6)
	
	//-NVIC_Clearpending
	NVIC_ClearPendingIRQ(ADC_IRQn);
	//-NVIC_Setpriority	
	NVIC_SetPriority(ADC_IRQn, 0);
	//-NVIC_Enable IRQ()
	NVIC_EnableIRQ(ADC_IRQn);
}

void LightSensorEnable() {
	*AT91C_ADCC_IER = 1 << 24; // Enable interrupts
	*AT91C_ADCC_CR = 1 << 1; //Begins analog-to-digital conversion
}

void LightSensorDisable() {
	*AT91C_ADCC_SR; // Dummy read
    *AT91C_ADCC_IDR = 1 << 24;
}

float *LightSensorGet() {
	float lights[2];
	lights[1] = ((*AT91C_ADCC_CDR2 * 3.3)/0xF);
	lights[0] = ((*AT91C_ADCC_CDR1 * 3.3)/0xF);
	return lights;
}

lightsensor Light = {
	.init = LightSensorInit,
	.get = LightSensorGet,
	.enable = LightSensorEnable,
	.disable = LightSensorDisable
};
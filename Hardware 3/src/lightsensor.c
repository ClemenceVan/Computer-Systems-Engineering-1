#include "lightsensor.h"

void LightSensorInit() {
	*AT91C_PMC_PCER1 = (1<<5); //Enable PMC for ADC
	*AT91C_PIOA_PDR = 1 << 4 | 1 << 5; //Disable PIO control over pins
	*AT91C_ADCC_MR = (0xFFFF<<8); //Set the prescaler (ADCClock = MCK / ( (PRESCAL+1) * 2 )) (MCK = 4MHz)   PRESCAL= MCK/2*ADCClock-1 = 4MHz/2
	*AT91C_ADCC_CHER = 1 << 1; // Enable channel 1 (A5 & A6)
	
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

float LightSensorGet() {
	return (*AT91C_ADCC_LCDR & 0xFFF) / (4096/3.3);
}

lightsensor Light = {
	.init = LightSensorInit,
	.get = LightSensorGet,
	.enable = LightSensorEnable,
	.disable = LightSensorDisable
};
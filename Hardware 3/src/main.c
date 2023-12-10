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
int lightReady = 0;

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
	if (lightReady) {
		lightReady = 0;
		clearDisplay();
		displayPrintf("light: %f", (*AT91C_ADCC_LCDR&0xFFF) / (4096/3.3));
		*AT91C_ADCC_IER = 1 << 24;
		*AT91C_ADCC_CR = 1<<1; //Start an ADC in ADC_CR
	}
}
int position = 0;

void setPos(int angle) // page 991 in datasheet
{
	if (angle < 0 || angle > 180)
		angle = 0;
	position = angle;
	// 1650 = 0° 6600 = 180°
	*AT91C_PWMC_CH1_CDTYR = 1650 + (angle * 33);
}

void steer(int angle) {
	// add angle to current position
	if (angle < 0 || angle > 180)
		return;
	setPos(position + angle);
}

void ServoInit(void)
{
	*AT91C_PMC_PCER1 = (1<<4);//init PWM
	
	// give control to peripheral PWM
	*AT91C_PIOB_PDR = (1<<17);
	*AT91C_PIOB_MDDR = (1<<17);  
	*AT91C_PIOB_ABMR = (1<<17);
	
	*AT91C_PWMC_ENA = (1<<1); //enable PWM
	
	*AT91C_PWMC_CH1_CMR = 0x5;//set prescaler (clk/32)
	
	*AT91C_PWMC_CH1_CPRDR = 52500; //set period

	setPos(0);
}

void ADC_Handler(void) {
	lightReady = 1;
	*AT91C_ADCC_IDR = 0xFFFFFFFF;
	*AT91C_ADCC_SR;
}

void init(void) {
    SystemInit();
	// Enable PIO
	*AT91C_PMC_PCER = (1 << 13);
	*AT91C_PMC_PCER = (1 << 12);
	
	// Enable peripherals pins
	KeypadInit();
	DisplayInit();
    // TempSensorInit();
	ServoInit();
	InitTimer(10, handle);

	// ADC
	*AT91C_PMC_PCER1 = (1<<5); //Enable PMC for ADC
	*AT91C_PIOA_PDR = 1 << 4 | 1 << 5; //Disable PIO control over pins
	*AT91C_ADCC_MR = (0xFFFF<<8); //Set the prescaler (ADCClock = MCK / ( (PRESCAL+1) * 2 )) (MCK = 4MHz)   PRESCAL= MCK/2*ADCClock-1 = 4MHz/2
	*AT91C_ADCC_IER = 1 << 24; // Enable interrupts
	*AT91C_ADCC_CHER = 1 << 1; // Enable channel 1 (A5 & A6)
	*AT91C_ADCC_CR = 1 << 1; //Begins analog-to-digital conversion
	*AT91C_ADCC_SR; // Dummy read
	
	//-NVIC_Clearpending
	NVIC_ClearPendingIRQ(ADC_IRQn);
	//-NVIC_Setpriority	
	NVIC_SetPriority(ADC_IRQn, 0);
	//-NVIC_Enable IRQ()
	NVIC_EnableIRQ(ADC_IRQn);

	// TempStart();
	Init_Display();
	
}

void PIOD_Handler(void) {
	if((*AT91C_PIOD_PDSR & (1 << 1)) == 0)
		steer(10);
    else if((*AT91C_PIOD_PDSR & (1 << 9)) == 0)
		steer(20);
    *AT91C_PIOD_ISR;
}

void main(void) {
	init();

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
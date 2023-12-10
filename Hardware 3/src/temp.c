#include "include.h"

void TempStartReset(void) {
}

void TempSensorInit(void) {
    *AT91C_PMC_PCER = (1<<27); //TC0

	*AT91C_PIOA_PDR = 1 << 17 | 1 << 18;

	*AT91C_PIOB_OER = (1<<25);
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR & 0x0);
	*AT91C_TC0_CCR = (1<<0); //enable clock
	*AT91C_TC0_CCR = (*AT91C_TC0_CCR|(1<<2)); //enable counter
	//Load counter to A when TIOA falling (TC_CMR0)
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR | (0x2<<16));
	//Load counter to B when TIOA falling (TC_CMR0)
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR | (0x2<<18));

	NVIC_ClearPendingIRQ(TC0_IRQn);
	NVIC_SetPriority(TC0_IRQn,0);
	NVIC_EnableIRQ(TC0_IRQn);
    TempStartReset();
    
   //Clear Line to Reset
   *AT91C_PIOB_CODR = (1<<25);
   Delay(10); //t_reset
    
   //setup pulse high
   *AT91C_PIOB_SODR = (1<<25);
   Delay(500);
}

void TempStart(void) {
    //0: no effect, 1: counter reset and clock start
    *AT91C_TC0_CCR = (*AT91C_TC0_CCR|(1<<2));
    
    
    //enable PIN25 as ouput
    *AT91C_PIOB_OER = (1<<25);
    
    //start pulse
    *AT91C_PIOB_CODR = (1<<25);
    Delay(1);  
    
    //ODR next to enable input (PIN25)
    *AT91C_PIOB_ODR = (1<<25);

    //enable Interrupts
    *AT91C_TC0_IER = AT91C_TC_LDRBS; 
}

float GetTemp(void) {
  //disable interrupts
  *AT91C_TC0_IDR = AT91C_TC_LDRBS;
  
  //calc temperature and return
  int rb = *AT91C_TC0_RB;
  int ra = *AT91C_TC0_RA;
  double ret = (rb - ra);
  ret = ret / 1.895; 
  ret = ret / (5.0);
  *AT91C_TC0_RB = 0x0;
  *AT91C_TC0_RA = 0x0;
  return ret/(22.4)-273.15;
}
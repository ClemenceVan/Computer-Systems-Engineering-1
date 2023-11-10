#include "system_sam3x.h"
#include "at91sam3x8.h"

#define PIO_AIMER (AT91_CAST(AT91_REG *) 0x400E14B0)
#define PIO_IFER (AT91_CAST(AT91_REG *) 0x400E1420)
#define PIO_DIFSR (AT91_CAST(AT91_REG *) 0x400E1484)
#define PIO_SCDR (AT91_CAST(AT91_REG *) 0x400E148C)

int ms = 0;
int blinking = 0;

void ReadButton(unsigned int* nButton) {
    if((*AT91C_PIOD_PDSR & (1 << 1)) == 0)
        *nButton = 1;
    else
        *nButton = 0;
}

void SetLed(int led) {
  if(led)
    *AT91C_PIOD_SODR = (1 << 3);
  else
    *AT91C_PIOD_CODR = (1 << 3);
}

void SysTick_Handler(void) {
    ms++;

    if (!blinking) return;
    if(ms % 1000 == 0) {
        static int led = 0;
        led = !led;
        SetLed(led);
    }
}

void PIOD_Handler(void) {
    // static int led = 0;
    // led = !led;
    // SetLed(led);
    blinking = !blinking;

    *AT91C_PIOD_ISR;
}

void main(void){
    SystemInit();

    *AT91C_PMC_PCER = (1 << 14);
    *AT91C_PIOD_PER = (1 << 1);
    *AT91C_PIOD_PER = (1 << 3);
    
    *AT91C_PIOD_ODR = (1 << 1);
    *AT91C_PIOD_OER = (1 << 3);
    
    *AT91C_PIOD_PPUER = (1 << 1);
    *AT91C_PIOD_PPUDR = (1 << 3);

    *PIO_AIMER = (1 << 1);
    *PIO_IFER = (1 << 1);
    *PIO_DIFSR = (1 << 1);

    *PIO_SCDR = (1 << 1);

    *AT91C_PIOD_ISR;
    NVIC_ClearPendingIRQ(PIOD_IRQn);
    NVIC_SetPriority(PIOD_IRQn, 0);
    NVIC_EnableIRQ(PIOD_IRQn);
    
    *AT91C_PIOD_IER = (1 << 1);
    int buttonVal;
    SysTick_Config((SystemCoreClock / 1000));
    while(1){
    //   ReadButton(&buttonVal);
    //   SetLed(buttonVal);
    }
}
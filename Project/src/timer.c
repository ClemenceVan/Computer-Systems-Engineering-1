#include "timer.h"

#define PIO_AIMER (AT91_CAST(AT91_REG *) 0x400E14B0)
#define PIO_IFER (AT91_CAST(AT91_REG *) 0x400E1420)
#define PIO_DIFSR (AT91_CAST(AT91_REG *) 0x400E1484)
#define PIO_SCDR (AT91_CAST(AT91_REG *) 0x400E148C)

struct parametrs {
    int interval;
    void (*callback)(void);
} timerParams = {0, 0};

void SysTick_Handler() {
    Timer.ms++;
    if(Timer.ms % timerParams.interval == 0) timerParams.callback();
}

void InitTimer(int interval, void (*callback)(void)) {
    timerParams.callback = callback;
    timerParams.interval = interval;
    *PIO_AIMER = (1 << 1);
    *PIO_IFER = (1 << 1);
    *PIO_DIFSR = (1 << 1);
    SysTick_Config((SystemCoreClock / 10));
}

timer Timer = {
    .init = InitTimer,
    .ms = 0,
    .Flags = {
        .temp = 0,
        .light = 0,
    }
};
#include "./timer.h"
#include "../calendar.h"

struct parametrs {
    int interval;
    void (*callback)(void);
} timerParams = {0, 0};

void SysTick_Handler() {
    Timer.ms++;
    if (Timer.ms % 10 == 0) Calendar.now++;
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
    .ms = 0
};
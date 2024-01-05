#include "./timer.h"
#include "../calendar.h"

struct parametrs {
    int interval;
    void (*callback)(void);
} timerParams = {0, 0};

void SysTick_Handler() {
    Timer.ds++;
    if (Timer.ds % 10 == 0) Calendar.now += Master.params.fastMode ? 1800 : 1;
    if (Timer.ds % timerParams.interval == 0) timerParams.callback();
}

void InitTimer(int interval, void (*callback)(void)) {
    timerParams.callback = callback;
    timerParams.interval = interval;
    *PIO_AIMER = (1 << 1);
    *PIO_IFER = (1 << 1);
    *PIO_DIFSR = (1 << 1);
    SysTick_Config((SystemCoreClock / 10)); // 10ds
}

timer Timer = {
    .init = InitTimer,
    .ds = 0
};
#pragma once
#include "../lib/system_sam3x.h"
#include "../lib/at91sam3x8.h"

#include <stdio.h>
#include <stdarg.h>

#define IDLE 0
#define READING 1
#define READY 2

typedef struct {
    int temp;
    int light;
    int keypad;
    char *alert;
} Flags;

typedef struct {
    int tempLimits[2];
    int fastMode;
} Params;

typedef struct {
    Flags flags;
    Params params;
} master;

extern master Master;

void Delay(int time);
void handle();

#define PIOC_ESR (AT91_CAST(AT91_REG *) 0x400E12C0) // PIOC Edge Select Register
#define PIOC_AIMER (AT91_CAST(AT91_REG *) 0x400E12B0) // PIOC Additional Interrupt Modes Enable Register
#define PIOC_DIFSR (AT91_CAST(AT91_REG *) 0x400E1284) // PIOC Debouncing Input Filter Select Register
#define PIOC_FRLHSR (AT91_CAST(AT91_REG *) 0x400E12D8) // PIOC Fall/Rising/Low/High Status Register
#define PIOC_REHLSR (AT91_CAST(AT91_REG *) 0x400E12D4) // PIOC Rising Edge/High Level Select Register
#define PIOC_FELLSR (AT91_CAST(AT91_REG *) 0x400E12D0) // PIOC Falling Edge/Low Level Select Register
#define PIOC_AIMDR (AT91_CAST(AT91_REG *) 0x400E12B4) // PIOC Additional Interrupt Modes Disables Register
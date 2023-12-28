#pragma once
#include "../include.h"

#define PIO_AIMER (AT91_CAST(AT91_REG *) 0x400E14B0)
#define PIO_IFER (AT91_CAST(AT91_REG *) 0x400E1420)
#define PIO_DIFSR (AT91_CAST(AT91_REG *) 0x400E1484)
#define PIO_SCDR (AT91_CAST(AT91_REG *) 0x400E148C)

typedef struct {
    int ms;
    void (*init)(int interval, void (*callback)(void));
} timer;

extern timer Timer;
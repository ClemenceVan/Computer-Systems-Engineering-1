#pragma once
#include "../include.h"

typedef struct temp_s {
    int initialized;
    int enabled;
    void(*init)(void);
    void(*enable)(void);
    void(*disable)(void);
    void (*start)(void);
    void (*stop)(void);
    float(*get)(void);
} tempSensor;

extern tempSensor Temperature;
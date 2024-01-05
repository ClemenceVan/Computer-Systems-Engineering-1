#pragma once
#include "../include.h"

typedef struct temp_s {
    void(*init)(void);
    void (*start)(void);
    void (*stop)(void);
    float(*get)(void);
} tempSensor;

extern tempSensor Temperature;
#pragma once
#include "../include.h"

// void LightSensorInit(void);

// void LightSensorEnable(void);

// void LightSensorDisable(void);

typedef struct {
    void (*init)(void);
    float *(*get)(void);
    void (*enable)(void);
    void (*disable)(void);
} lightsensor;

extern lightsensor Light;
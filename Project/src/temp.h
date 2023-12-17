#include "include.h"

typedef struct temp_s {
    void(*init)(void);
    void(*enable)(void);
    void(*disable)(void);
    float(*get)(void);
} tempSensor;

extern tempSensor Temperature;
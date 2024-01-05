#pragma once
#include "../include.h"

typedef struct {
    void(*setPos)(int pos);
    void(*steer)(int angle);
    void(*init)(void);
    int position;
} servo;

extern servo Servo;
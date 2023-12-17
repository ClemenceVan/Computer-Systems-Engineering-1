#pragma once
#include "include.h"
#include "databus.h"

typedef struct {
    int(*poll)(void);
    void(*init)(void);
} keypad;

extern keypad Keypad;
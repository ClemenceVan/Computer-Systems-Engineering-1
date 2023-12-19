#include "include.h"
#include <time.h>
#include <stdio.h>

typedef struct {
    time_t now;
    Date (*getNow)(void);
    void (*setDateTime)(Date date);
    char* (*toString)(Date date);
} calendar;

extern calendar Calendar;
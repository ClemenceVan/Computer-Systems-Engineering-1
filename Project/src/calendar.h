#pragma once
#include "include.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TEMPERATURES 100 // TBD
#define READING_EXPIRATION 604800000 // 7 days in milliseconds

typedef struct {
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
} Time;

typedef struct {
    unsigned short day;
    unsigned short month;
    unsigned short year;
    Time time;
} Date;

struct node_t {
    time_t timestamp;
    float temperature;
    struct node_t* next;
};

typedef struct node_t Node;

typedef struct {
    time_t now;
    Node* temperatures;
    Date (*getNow)(void);
    time_t (*getEndOfDay)();
    Date (*getDateFromTimestamp)(time_t timestamp);
    void (*setDateTime)(Date date);
    char* (*toString)(Date date);
    void (*addTemperature)(float temperature);
    Node* (*getRecordings)(time_t from, time_t to);
    void (*freeList)(Node* startNode);
} calendar;

extern calendar Calendar;
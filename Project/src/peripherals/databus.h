#pragma once
#include "../include.h"

typedef struct databus_s {
	int dir;
	int OE;
	int data_pins[8];
} databus;

void SetDir(databus db, int dir);

void SetBusAsOutput(databus db, int value);

unsigned char ReadDatabus(databus db);

void SetDatabus(databus db, unsigned char value);
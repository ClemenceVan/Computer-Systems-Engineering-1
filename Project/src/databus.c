#include "databus.h"

void SetDir(databus db, int dir) {
	*(dir ? AT91C_PIOC_SODR : AT91C_PIOC_CODR) = (1 << db.dir);
}

void SetBusAsOutput(databus db, int value) {
	for (int i = 0; i < 8; i++) {
		if (value)
			*AT91C_PIOC_OER = (1 << db.data_pins[i]);
		else
			*AT91C_PIOC_ODR = (1 << db.data_pins[i]);
	}
}

unsigned char ReadDatabus(databus db) {
	return (*AT91C_PIOC_PDSR >> 2) & 0xFF;
}

void SetDatabus(databus db, unsigned char value) {
	*AT91C_PIOC_CODR = (0xFF << 2);
	*AT91C_PIOC_SODR = (value << 2);
}
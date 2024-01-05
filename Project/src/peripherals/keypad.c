#include "./keypad.h"

int pollPanel() {
	int value = 0;
	// Make all Column pin as high output
	*AT91C_PIOC_OER = (1 << 7) | (1 << 8) | (1 << 9);
	*AT91C_PIOC_SODR = (1 << 7) | (1 << 8) | (1 << 9);
	for(int col = 0; col < 3; col++) {
		*AT91C_PIOC_CODR = (1 << (col + 7)); // Clear one column at the time
		for(int row = 1; row <= 4; row++) // Loop Row
			if((*AT91C_PIOC_PDSR & (1 << (row + 1))) == 0)// Read row and check if bit is zero
				return value = (row == 4 ? 0 : row) * 3 + (col == 0 ? 3 : col);
		*AT91C_PIOC_SODR = (1 << (col + 7)); // Set the column again
	}
	*AT91C_PIOC_CODR = (1 << 7) | (1 << 8) | (1 << 9);
	return value;
}

void KeypadInit() {
	// Configure OEBA pin as output
	*AT91C_PIOD_PER = (1 << 2);
	*AT91C_PIOD_OER = (1 << 2);
    // Configure COL pins as output
    *AT91C_PIOC_PER = (1 << 7) | (1 << 8) | (1 << 9);
    *AT91C_PIOC_OER = (1 << 7) | (1 << 8) | (1 << 9);
    *AT91C_PIOC_PPUDR = (1 << 7) | (1 << 8) | (1 << 9);
    // Configure ROW pins as input
    *AT91C_PIOC_PER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    *AT91C_PIOC_ODR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	// keypad interrupts
    *AT91C_PIOC_PPUDR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    *AT91C_PIOC_IER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_AIMER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_ESR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_FELLSR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	NVIC_ClearPendingIRQ(PIOC_IRQn);
	NVIC_SetPriority(PIOC_IRQn, 0);
	NVIC_EnableIRQ(PIOC_IRQn);
	*AT91C_PIOC_ISR;
}

keypad Keypad = {
	.poll = pollPanel,
	.init = KeypadInit,
	.key = 0
};
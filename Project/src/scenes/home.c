#include "scenes.h"

int homeScene() {
	Display.printfAt((int[2]){(DISPLAY_WIDTH/2)-6, DISPLAY_HEIGHT/2}, " SMART HOME ");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[1]Recordings [2]Settings");
	while (1) {
		if (Keypad.key > 0 && Keypad.key < 5) return Keypad.key;
		Delay(1000000);
		Display.printfAt((int[2]){DISPLAY_WIDTH - 19, 0}, "%s", Calendar.toString(Calendar.getNow()));
	}
}
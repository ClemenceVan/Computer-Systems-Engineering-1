#include "scenes.h"

int recordingsScene() {
	Display.printfAt((int[2]){0, 0}, "RECORDINGS");
	Display.printfAt((int[2]){0, 1}, "Latest Recorded temp:");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back");
	while (1) {
		if (Keypad.key == 11) return 0;
		Delay(1000000);
		Display.printfAt((int[2]){DISPLAY_WIDTH - 19, 0}, "%s", Calendar.toString(Calendar.getNow()));
	}
}
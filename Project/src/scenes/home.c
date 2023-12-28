#include "scenes.h"

int homeScene() {
	Display.printfAt((int[2]){(DISPLAY_WIDTH/2)-6, DISPLAY_HEIGHT/2}, " SMART HOME ");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[1]Recordings [2]Settings");
	int blink = 0;
	while (1) {
		if (Keypad.key > 0 && Keypad.key < 5) return Keypad.key;
		Delay(1000000);
		Display.printfAt((int[2]){DISPLAY_WIDTH - 19, 0}, "%s", Calendar.toString(Calendar.getNow()));
		if (Master.flags.alert) {
			if (blink)
				Display.printfAt((int[2]){0, 0}, "%s", Master.flags.alert);
			else
				Display.printfAt((int[2]){0, 0}, "                     ");
			blink = !blink;
			Display.printfAt((int[2]){0, 1}, "[*] to dismiss");
			if (Keypad.key == 10) {
				Keypad.key = 0;
				free(Master.flags.alert);
				Master.flags.alert = NULL;
				*AT91C_PIOB_CODR = 1 << 27;
				Display.printfAt((int[2]){0, 1}, "                        ");
				Display.printfAt((int[2]){0, 2}, "                ");
			}
		}
	}
}
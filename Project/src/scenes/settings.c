#include "scenes.h"

int settingsScene() {
	Display.printfAt((int[2]){0, 0}, "SETTINGS");
	Display.printfAt((int[2]){0, 2}, "[1] Set Date/Time");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back");
	int centerX = DISPLAY_WIDTH / 2;
	int centerY = DISPLAY_HEIGHT / 2;
	while (1) {
		if (Keypad.key == 1) {
			Keypad.key = 0;
			int pos = centerX - 10;
			printf(">%d\n", pos);
			int blink = 0;
			int ticks = 0;
			char *buffer = Calendar.toString(Calendar.getNow());
			Display.printfAt((int[2]){centerX - 12, centerY - 2}, "+---------------------+");
			for (int i = 1; i < 5; i++) {
				Display.printfAt((int[2]){centerX - 12, centerY - 2 + i}, "|                     |");
			}
			Display.printfAt((int[2]){centerX - 10, centerY}, buffer);
			Display.printfAt((int[2]){centerX - 12, centerY + 2}, "+---------------------+");
			Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back [*]Next [#]Previous");
			while(1) {
				ticks++;
				if (ticks > 3000000) {
					ticks = 0;
					Display.printfAt((int[2]){pos, centerY + 1}, blink ? " " : "-");
					blink = !blink;
				}
				if (Keypad.key) {
					if (Keypad.key == 10 || Keypad.key == 12) {
						Display.printfAt((int[2]){pos, centerY + 1}, " ");
						if (Keypad.key == 10) {
							if (pos >= centerX + 8) { Calendar.setDateTime((Date) {
								.day = (buffer[0] - 48) * 10 + (buffer[1] - 48),
								.month = (buffer[3] - 48) * 10 + (buffer[4] - 48),
								.year = (buffer[6] - 48) * 1000 + (buffer[7] - 48) * 100 + (buffer[8] - 48) * 10 + (buffer[9] - 48),
								.time = (Time) {
									.hour = (buffer[11] - 48) * 10 + (buffer[12] - 48),
									.minute = (buffer[14] - 48) * 10 + (buffer[15] - 48),
									.second = (buffer[17] - 48) * 10 + (buffer[18] - 48)
								}
							}); break;}
							pos += 1;
						}
						else {
							if (pos <= centerX - 10) break;
							pos -= 1;
						}
						blink = 0;
						printf (">%c %d %d %d\n", buffer[pos - 10], buffer[pos - 10], pos, pos - 10);
						if (buffer[pos - 10] < 48 || buffer[pos - 10] > 57){ Keypad.key == 10 ? pos++ : pos--; printf ("skipped\n"); }
						Display.printfAt((int[2]){pos, centerY + 1}, "-");
					}
					// else if (Keypad.key == 11) {
					// 	return settingsScene();
					// }
					else {
						int valid = 1;
						char temp = buffer[pos - 10];
						if (Keypad.key == 11) Keypad.key = 0;
						buffer[pos - 10] = Keypad.key + 48;
						int month = (buffer[3] - 48) * 10 + (buffer[4] - 48);
						switch (pos - 10) {
							case 0:
							case 1:
								int day = (buffer[0] - 48) * 10 + (buffer[1] - 48);
								if (month == 2) valid = day < 29 && day > 0;
								else if (month == 4 || month == 6 || month == 9 || month == 11) valid = day < 31 && day > 0;
								if (day >= 30 && day <= 39) {
									buffer[1] = '0';
									day = 30;
								}
								else valid = day < 32 && day > 0;
								printf("%d\n", valid);
								break;
							case 3:
							case 4:
								if (buffer[0] == '3' && buffer[1] == '1' && (month == 4 || month == 6 || month == 9 || month == 11))
									buffer[1] = '0';
								if (month == 2 && (buffer[0] - 48) * 10 + (buffer[1] - 48) > 28) {
									buffer[0] = '2';
									buffer[1] = '8';
								}
								month = (buffer[3] - 48) * 10 + (buffer[4] - 48);
								valid = month < 13 && month > 0;
								break;
							case 6:
							case 7:
							case 8:
							case 9:
								int year = (buffer[6] - 48) * 1000 + (buffer[7] - 48) * 100 + (buffer[8] - 48) * 10 + (buffer[9] - 48);
								valid = year < 2037 && year > 1970;
								break;
							case 11:
							case 12:
								int hour = (buffer[11] - 48) * 10 + (buffer[12] - 48);
								valid = hour < 24 && hour >= 0;
								break;
							case 13:
							case 14:
							case 16:
							case 17:
								int minute = (buffer[14] - 48) * 10 + (buffer[15] - 48);
								valid = pos - 10 != 16 && minute < 60 && minute >= 0;
								break;
						}
						if (!valid)
							buffer[pos - 10] = temp;
						Display.printfAt((int[2]){centerX - 10, centerY}, buffer);
					}
					Keypad.key = 0;
				}
			}
			Display.clear();
			Display.printfAt((int[2]){0, 0}, "SETTINGS");
			Display.printfAt((int[2]){0, 2}, "[1] Set Date/Time");
			Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back");
		}
		else if (Keypad.key == 11) return 0;
	}
}
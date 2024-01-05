#include "scenes.h"

#define CENTER_X (DISPLAY_WIDTH / 2)
#define CENTER_Y (DISPLAY_HEIGHT / 2)

void calendarSettings() {
	Display.printfAt((int[2]){0, 2}, "> [1] Set Date/Time <");
	int pos = CENTER_X - 10;
	int blink = 0;
	int ticks = 0;
	char *buffer = Calendar.toString(Calendar.getNow());
	Display.printfAt((int[2]){CENTER_X - 12, CENTER_Y - 2}, "+---------------------+");
	for (int i = 1; i < 5; i++)
		Display.printfAt((int[2]){CENTER_X - 12, CENTER_Y - 2 + i}, "|                     |");
	Display.printfAt((int[2]){CENTER_X - 10, CENTER_Y}, buffer);
	Display.printfAt((int[2]){CENTER_X - 12, CENTER_Y + 2}, "+---------------------+");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT - 2}, "[#] start of line to cancel");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT - 1}, "[*] end of line to save");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[#]Previous [*]Next");
	while(1) {
		ticks++;
		if (ticks > 3000000) {
			ticks = 0;
			Display.printfAt((int[2]){pos, CENTER_Y + 1}, blink ? " " : "-");
			blink = !blink;
		}
		if (Keypad.key) {
			if (Keypad.key == 10 || Keypad.key == 12) {
				Display.printfAt((int[2]){pos, CENTER_Y + 1}, " ");
				if (Keypad.key == 10) {
					if (pos >= CENTER_X + 8) { Calendar.setDateTime((Date) {
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
					if (pos <= CENTER_X - 10) break;
					pos -= 1;
				}
				blink = 0;
				printf (">%c %d %d %d\n", buffer[pos - 10], buffer[pos - 10], pos, pos - 10);
				if (buffer[pos - 10] < 48 || buffer[pos - 10] > 57) Keypad.key == 10 ? pos++ : pos--;
				Display.printfAt((int[2]){pos, CENTER_Y + 1}, "-");
			}
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
				Display.printfAt((int[2]){CENTER_X - 10, CENTER_Y}, buffer);
			}
			Keypad.key = 0;
		}
	}
}

void temperatureSettings() {
	Display.printfAt((int[2]){0, 3}, "> [2] Set Temperature Limits <");
	int blink = 0;
	int ticks = 0;
	int pos = 0;
	char *cursor = NULL;
	int lengths[2] = {
		snprintf(NULL, 0, "%d", Master.params.tempLimits[0]),
		snprintf(NULL, 0, "%d", Master.params.tempLimits[1])
	};
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Save [*]Next [#]Prev. [8]Up [2]Down");
	Display.printfAt((int[2]){CENTER_X - 12, CENTER_Y - 2}, "+---------------------+");
	for (int i = 1; i < 5; i++) Display.printfAt((int[2]){CENTER_X - 12, CENTER_Y - 2 + i}, "|                     |");
	Display.printfAt((int[2]){CENTER_X - 1 - (lengths[pos] + 1), CENTER_Y}, "%d / %d", Master.params.tempLimits[0], Master.params.tempLimits[1]);
	Display.printfAt((int[2]){CENTER_X - 12, CENTER_Y + 2}, "+---------------------+");
	while (1) {
		ticks++;
		if (ticks > 3000000) {
			ticks = 0;
			if (cursor) free(cursor);
			cursor = malloc(sizeof(char) * (lengths[pos] + 1));
			for (int i = 0; i < lengths[pos]; i++) cursor[i] = blink ? ' ' : '-';
			cursor[lengths[pos]] = '\0';
			Display.printfAt((int[2]){(CENTER_X - 1 - (lengths[0] + 1)) + (3 + lengths[0]) * pos, CENTER_Y + 1}, "%s", cursor);
			blink = !blink;
		}
		if (Keypad.key) {
			switch (Keypad.key) {
				case 10:
				case 12:
					pos = Keypad.key == 10 ? 1 : 0;
					Display.printfAt((int[2]){CENTER_X - 5, CENTER_Y + 1}, "         ");
					break;
				case 8:
				case 2:
					if (pos == 0 && Master.params.tempLimits[0] + (Keypad.key == 8 ? 1 : -1) >= Master.params.tempLimits[1]) break;
					else if (pos == 1 && Master.params.tempLimits[1] + (Keypad.key == 8 ? 1 : -1) <= Master.params.tempLimits[0]) break;
					Master.params.tempLimits[pos] += Keypad.key == 8 ? 1 : -1;
					if (Master.params.tempLimits[pos] > 99) Master.params.tempLimits[pos] = 99;
					else if (Master.params.tempLimits[pos] < -99) Master.params.tempLimits[pos] = -99;
					lengths[pos] = snprintf(NULL, 0, "%d", Master.params.tempLimits[pos]);
					Display.printfAt((int[2]){CENTER_X - 9, CENTER_Y}, "             ");
					Display.printfAt((int[2]){CENTER_X - 2 - lengths[0], CENTER_Y}, "%d / %d", Master.params.tempLimits[0], Master.params.tempLimits[1]);
					break;
				case 11:
					return;
					break;
				default:
					break;
			}
			Keypad.key = 0;
		}
	}
	
}

int settingsScene() {
	while (1) {
		Keypad.key = 0;
		Display.clear();
		Display.printfAt((int[2]){0, 0}, "    SETTINGS");
		Display.printfAt((int[2]){0, 2}, "[1] Set Date/Time");
		Display.printfAt((int[2]){0, 3}, "[2] Set Temperature Limits");
		Display.printfAt((int[2]){0, 4}, "[3] %s Fast Mode", Master.params.fastMode ? "Disable" : "Enable");
		Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back");
		while (!Keypad.key);
		switch (Keypad.key) {
			case 1:
				calendarSettings();
				break;
			case 2:
				temperatureSettings();
				break;
			case 3:
				Master.params.fastMode = !Master.params.fastMode;
				break;
			case 11:
				return 0;
		}
	}
}
#include "scenes.h"
#include "../peripherals/temp.h"

void displayData(int scroll) {
	Display.printfAt((int[2]){0, 0}, "    RECORDINGS");
	Display.printfAt((int[2]){0, 1}, "----------------------------------------");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back [*]Next [#]Previous");
	Display.printfAt((int[2]){DISPLAY_WIDTH - 1, 3}, "-");
	Display.printfAt((int[2]){DISPLAY_WIDTH - 1, 4 + scroll}, "|");
	Display.printfAt((int[2]){DISPLAY_WIDTH - 1, 5 + scroll}, "|");
	Display.printfAt((int[2]){DISPLAY_WIDTH - 1, 13}, "-");
	int pos = 0;
	int i = 0;
	time_t now = Calendar.getEndOfDay();
	for (i = 0; pos < 12 && scroll + i < 8; i++) { // get recordings for each day between 00:00 and 23:59
		Node *node = Calendar.getRecordings(now - ((scroll + i) * 86400), (now - ((scroll + i) * 86400) - 86400));
		// printf("%lld, %lld\n", now - ((scroll + i) * 86400), (now - ((scroll + i) * 86400) - 86400));
		if (node) {
			Node *temp = node;
			float average = 0;
			int count = 0;
			float max = 0;
			int maxTime[2] = {0, 0}; // [0] = hour, [1] = minute
			float min = 100;
			int minTime[2] = {0, 0}; // [0] = hour, [1] = minute
			Date day;
			day = Calendar.getDateFromTimestamp(node->timestamp);
			while (temp) {
				average += temp->temperature;
				count++;
				if (temp->temperature > max) {
					max = temp->temperature;
					maxTime[0] = Calendar.getDateFromTimestamp(temp->timestamp).time.hour;
					maxTime[1] = Calendar.getDateFromTimestamp(temp->timestamp).time.minute;
				}
				if (temp->temperature < min) {
					min = temp->temperature;
					minTime[0] = Calendar.getDateFromTimestamp(temp->timestamp).time.hour;
					minTime[1] = Calendar.getDateFromTimestamp(temp->timestamp).time.minute;
				}
				temp = temp->next;
			}
			average /= count;
			Display.printfAt((int[2]){0, 3 + pos}, "%02d/%02d/%04d  Average temperature %.2fC", day.day, day.month, day.year, average);
			Display.printfAt((int[2]){12, 4 + pos}, "Max. %.2fC at %.2d:%.2d", max, maxTime[0], maxTime[1]);
			Display.printfAt((int[2]){12, 5 + pos}, "Min. %.2fC at %.2d:%.2d", min, minTime[0], minTime[1]);
			pos += 4;
		} else {
			Date day = Calendar.getDateFromTimestamp(now - ((scroll + i) * 86400));
			Display.printfAt((int[2]){0, 3 + pos}, "%02d/%02d/%04d  No recordings for this day", day.day, day.month, day.year);
			pos += 4;
		}
		Calendar.freeList(node);
	}
}

int recordingsScene() {
	int scroll = 0;
	displayData(scroll);
	while (1) {
		Delay(1000000);
		if (Keypad.key) {
			switch (Keypad.key) {
				case 10:
					scroll++;
					if (scroll > 7) scroll = 7;
					break;
				case 12:
					scroll--;
					if (scroll < 0) scroll = 0;
					break;
				case 11:
					return 0;
				default:
					continue;
					break;
			}
			Keypad.key = 0;
			Display.clear();
			displayData(scroll);
			printf("\n");
		}
	}
}
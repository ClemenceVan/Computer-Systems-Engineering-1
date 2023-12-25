#include "include.h"
#include "databus.h"
#include "keypad.h"
#include "display.h"
#include "timer.h"
#include "temp.h"
#include "lightsensor.h"
#include "servo.h"
#include "calendar.h"

#define IDLE 0
#define READY 1
#define AUTO_RECORDING 2
#define AUTO_READY 3

int key = 0;
void Delay(int value) {
	for (int i = 0; i < value; i++)
		asm("nop");
}

void TC0_Handler(void) {
	if (Timer.Flags.temp == AUTO_RECORDING) Timer.Flags.temp = AUTO_READY;
	else Timer.Flags.temp = READY;
	Temperature.disable();
}

void ADC_Handler(void) {
	Timer.Flags.light = 1;
	Light.disable();
}

typedef struct snake_t {
	int x;
	int y;
	struct snake_t *next;
} Snake;

void snakeIncrease(Snake *snake) {
	Snake *new = malloc(sizeof(Snake));
	Snake *last = snake;
	new->x = snake->x;
	new->y = snake->y;
	for (; last->next != 0; last = last->next);
	last->next = new;
	new->next = 0;
}

void moveSnake(Snake *snake) {
	int pos[2] = {snake->x, snake->y};
	for (Snake *s = snake->next; s != 0; s = s->next) {
		int temp[2] = {s->x, s->y};
		s->x = pos[0];
		s->y = pos[1];
		pos[0] = temp[0];
		pos[1] = temp[1];
	}
}

void clearSnake(Snake *snake) {
	for (Snake *s = snake; s != 0; s = s->next) {
		Display.printfAt((int[2]){s->x, s->y}, " ");
	}
}

int *spawnFood() {
	static int food[2] = {0, 0};
	food[0] = rand() % DISPLAY_WIDTH;
	food[1] = rand() % DISPLAY_HEIGHT;
	printf("%d %d\n", food[0], food[1]);
	Display.printfAt((int[2]){food[0], food[1]}, "X");
	return food;
}

int gameScene() {
	Snake snake = {
		.x = 0,
		.y = 0,
		.next = 0
	};
	int direction = 2; // 1 - up, 2 - right, 3 - down, 4 - left
	int gameover = 0;
	int *food = 0;
	srand(Calendar.now);
    while (!gameover) {
		Delay(5000000);
		switch (key) {
			case 1:
				direction = 2;
				break;
			case 2:
				direction = 3;
				break;
			case 3:
				direction = 4;
				break;
			case 5:
				direction = 1;
				break;
		}
		if (food == 0) food = spawnFood();
		clearSnake(&snake);
		moveSnake(&snake);
		switch (direction) {
			case 1:
				snake.y = snake.y - 1;
				break;
			case 2:
				snake.x = snake.x + 1;
				break;
			case 3:
				snake.y = snake.y + 1;
				break;
			case 4:
				snake.x = snake.x - 1;
				break;
		}
		if (snake.x < 0 || snake.x >= DISPLAY_WIDTH || snake.y < 0 || snake.y >= DISPLAY_HEIGHT) {
			Display.printfAt((int[2]){(DISPLAY_WIDTH/2)-6, DISPLAY_HEIGHT/2}, " GAME OVER ");
			gameover = 1;
		}
		if (snake.x == food[0] && snake.y == food[1]) {
			food = 0;
			snakeIncrease(&snake);
		}
		for (Snake *s = snake.next; s != 0; s = s->next) {
			Display.printfAt((int[2]){s->x, s->y}, "#");
		}
		Display.printfAt((int[2]){snake.x, snake.y}, "O");
    }
	while(key != 11);
	return 0;
}


int homeScene() {
	Display.printfAt((int[2]){(DISPLAY_WIDTH/2)-6, DISPLAY_HEIGHT/2}, " SMART HOME ");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[1]Calendar ");
	Display.printfAt((int[2]){13, DISPLAY_HEIGHT}, "[2]Recordings ");
	Display.printfAt((int[2]){29, DISPLAY_HEIGHT}, "[3]Settings ");
	while (1) {
		if (key > 0 && key < 5) return key;
	}
}

int calendarScene() {
	Display.printfAt((int[2]){0, 0}, "CALENDAR");
	Display.printfAt((int[2]){0, 1}, "dd/mm/yyyy");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back");
	while (1) {
		if (key == 11) return 0;
	}
}

int recordingsScene() {
	Display.printfAt((int[2]){0, 0}, "RECORDINGS");
	Display.printfAt((int[2]){0, 1}, "Latest Recorded temp:");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back");
	while (1) {
		if (key == 11) return 0;
	}
}

int settingsScene() {
	Display.printfAt((int[2]){0, 0}, "SETTINGS");
	Display.printfAt((int[2]){0, 2}, "[1] Set Date/Time");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[0]Back");
	while (1) {
		// if (Timer.Flags.keypad) {
			// Timer.Flags.keypad = 0;
			if (key == 1) {
				int pos = 0;
				char *buffer = Calendar.toString(Calendar.getNow());
				Display.printfAt((int[2]){DISPLAY_WIDTH - 10, DISPLAY_HEIGHT / 2}, buffer);
				while(1) {
					if (key == 11) return 0;
				}
			}
			else if (key == 11) return 0;
		// }
	}
}

static int (*screens[])(void) = {homeScene, calendarScene, recordingsScene, settingsScene, gameScene};

static int currentScreen = 0;


int switchScreen(int screen) {
	currentScreen = screen;
	Display.clear();
	if (screen < 0 || screen >= sizeof(screens)/sizeof(screens[0])) return -1;
	switchScreen(screens[screen]());
}

void handle() {
	// Watch for temperature recording
	// if (Temperature.initialized && elapsed % 60000 == 0) {
	// 	Timer.Flags.temp = AUTO_RECORDING;
	// 	Temperature.start();
	// 	elapsed = 0;
	// }

	// switch (Timer.Flags.temp) {
	// 	case IDLE:
	// 		break;
		
	// 	case READY:
	// 		// TBD
	// 		if (Temperature.enabled) Temperature.start();
	// 		break;
	// 	case AUTO_READY:
	// 		Calendar.addTemperture(Temperature.get()); // to be tested
	// 		if (Temperature.enabled) Temperature.start();
	// 		break;
	// 	default:
	// 		break;
	// }

	// if (Timer.Flags.temp != IDLE) {
	// 	if (Timer.Flags.temp == READY) {
	// 		// TBD
	// 	}
	// 	else if (Timer.Flags.temp == AUTO_READY) {
	// 		Display.printfAt((int[2]){0, 1}, "Latest Recorded temp: %.2f", Temperature.get());
	// 	}
	// 	Timer.Flags.temp = IDLE;
	// 	Temperature.disable();
	// }
	// Display.printfAt((int[2]){DISPLAY_WIDTH - 19, 0}, Calendar.toString(Calendar.getNow()));
}

void init(void) {
    SystemInit();
	// Enable PIO
	*AT91C_PMC_PCER = (1 << 13);
	*AT91C_PMC_PCER = (1 << 12);
	
	// Enable peripherals pins
	Keypad.init();
	// Servo.init();
	Display.init();
	// Temperature.init();
	// Light.init();

	// Temperature.enable();
	Timer.init(10, handle);
	// Light.enable();
	Display.enable();
	Display.clear();
}

void PIOD_Handler(void) {
	switch (*AT91C_PIOD_ISR) {
		case 1 << 1:
			Servo.steer(10);
			break;
		case 1 << 9:
			Servo.steer(20);
			break;
	}
}

void PIOC_Handler(void) {
	*AT91C_PIOC_IDR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	int t = Keypad.poll();
	key = t ? t : key;
	// Timer.Flags.keypad = 1;
	*AT91C_PIOC_ISR;
	*AT91C_PIOC_IER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
}



void main(void) {
	init();

	Display.printfAt((int[2]){(DISPLAY_WIDTH/2)-6, DISPLAY_HEIGHT/2}, " SMART HOME ");
	Display.printfAt((int[2]){0, DISPLAY_HEIGHT}, "[1]Calendar ");
	Display.printfAt((int[2]){13, DISPLAY_HEIGHT}, "[2]Recordings ");
	Display.printfAt((int[2]){29, DISPLAY_HEIGHT}, "[3]Settings ");

	// For buttons, to be cleaned up
    *AT91C_PMC_PCER = (1 << 14);
	*AT91C_PIOD_PER = 1 << 1 | 1 << 9;

	*AT91C_PIOD_ODR = 1 << 1 | 1 << 9;
    *AT91C_PIOD_PPUER = 1 << 1 | 1 << 9;

    *AT91C_PIOD_ISR;
    NVIC_ClearPendingIRQ(PIOD_IRQn);
    NVIC_SetPriority(PIOD_IRQn, 0);
    NVIC_EnableIRQ(PIOD_IRQn);
    
    *AT91C_PIOD_IER = 1 << 1 | 1 << 9;

	Calendar.setDateTime((Date) {
			.day = 21,
			.month = 12,
			.year = 2023,
			.time = (Time) {
				.hour = 13,
				.minute = 17,
				.second = 0
			}
		});
	
	// keypad interrupts, should be moved to keypad.c
    *AT91C_PIOC_PPUDR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
    *AT91C_PIOC_IER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_AIMER = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_ESR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	*PIOC_FELLSR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	NVIC_ClearPendingIRQ(PIOC_IRQn);
	NVIC_SetPriority(PIOC_IRQn, 0);
	NVIC_EnableIRQ(PIOC_IRQn);
	*AT91C_PIOC_ISR;

	
	while(1) {
		switchScreen(currentScreen);
    }
}
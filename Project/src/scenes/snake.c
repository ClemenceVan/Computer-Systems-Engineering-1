#include "scenes.h"

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
		// Delay(5000000);
		Delay(3000000);
		switch (Keypad.key) {
			case 1:
				direction = direction == 4 ? direction : 2;
				break;
			case 2:
				direction = direction == 1 ? direction : 3;
				break;
			case 3:
				direction = direction == 2 ? direction : 4;
				break;
			case 5:
				direction = direction == 3 ? direction : 1;
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
        for (Snake *s = snake.next; s != 0; s = s->next) {
            if (snake.x == s->x && snake.y == s->y) {
                Display.printfAt((int[2]){(DISPLAY_WIDTH/2)-6, DISPLAY_HEIGHT/2}, " GAME OVER ");
                gameover = 1;
            }
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
	while(Keypad.key != 11);
	return 0;
}
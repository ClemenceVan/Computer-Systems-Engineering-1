TASK1 = task1
TASK2 = task2
SRCS = complex.c
OBJS =
CFLAGS = -std=gnu99 -Wall -Wextra# -Werror
LFLAGS = -lm

all: $(TASK1) $(TASK2)

$(TASK1): $(OBJS)
	gcc -o $(TASK1) $(TASK1).c $(SRCS) $(OBJS) $(CFLAGS) $(LFLAGS)

$(TASK2): $(OBJS)
	gcc -o $(TASK2) $(TASK2).c $(SRCS) $(OBJS) $(CFLAGS) $(LFLAGS)

%.o: %.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(TASK1) $(TASK2)

re: clean all

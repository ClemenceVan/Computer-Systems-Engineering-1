MAINFILE = SW1
SRCS = *.c
OBJS =
CFLAGS = -std=gnu99 -Wall -Wextra
LFLAGS = -lm

all: $(MAINFILE)

$(MAINFILE): $(OBJS)
	gcc -o $(MAINFILE) $(SRCS) $(OBJS) $(CFLAGS) $(LFLAGS)

%.o: %.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(MAINFILE) $(OBJS)

re: clean all

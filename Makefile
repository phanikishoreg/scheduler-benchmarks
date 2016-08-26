CC := gcc
RM := rm
CFLAGS := -O3 -g

all: threads shm timer

%: %.c priority.c
	$(CC) $(CFLAGS) $< priority.c -o $@ -lrt -lpthread

clean:
	$(RM) -f *.o threads shm timer

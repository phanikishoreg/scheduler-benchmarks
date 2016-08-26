CC := gcc
RM := rm
CFLAGS := -O3 -g

all: threads shm timer pipe

%: %.c priority.c
	$(CC) $(CFLAGS) $< priority.c -o $@.out -lrt -lpthread

clean:
	$(RM) -f *.o *.out 

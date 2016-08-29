CC := gcc
RM := rm
CFLAGS := -O3 -g

all: threads processes timer pipe mmap 

%: %.c priority.c
	$(CC) $(CFLAGS) $< priority.c -o $@.out -lrt -lpthread

clean:
	$(RM) -f *.o *.out 

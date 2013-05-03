threads: threads.c priority.c
	cc -o $@ threads.c priority.c -lrt -lpthread

shm: shm.c priority.c
	cc -o $@ shm.c priority.c -lrt

timer: timer.c priority.c
	cc -o $@ timer.c priority.c -lrt

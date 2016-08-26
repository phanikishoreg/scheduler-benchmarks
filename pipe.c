#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PAGE_SIZE   4096
#define U_WAIT      100

extern void set_prio (unsigned int);

static __inline__ unsigned long long
rdtsc (void)
{
  unsigned long long int x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
  return x;
}

int
main(int argc, char **argv)
{
	int pipe_fd[2], data_fd;
	int write_delay;
	pid_t child;
	char data_buf[PAGE_SIZE];
	unsigned long long w_start, w_end, r_start, r_end;

	if (argc < 3) {
		printf("Usage: %s <input_file> <read/write>\n", argv[0]);
		printf("\t <input_file> : file containing data for this test\n");
		printf("\t <read/readblk/write> : read - ubench for read\t\treadblk - ubench for blocking read\t\twrite - ubench for write\n");
		return -1;
	}

	if (strcmp(argv[2], "readblk") == 0) write_delay = 1;
	else write_delay = 0;

	data_fd = open(argv[1], O_RDONLY);
	if (data_fd < 0) {
		perror("open");
		return -1;
	}
	memset(data_buf, 0, PAGE_SIZE);
	if (read(data_fd, data_buf, PAGE_SIZE-1) < 0) {
		perror("read");
		close (data_fd);
	}
	close (data_fd);
	
	if (pipe(pipe_fd) < 0) {
		perror("pipe");
		return -1;
	}

	if (write_delay == 0) {
		w_start = rdtsc();
		if (write(pipe_fd[1], data_buf, strlen(data_buf)) < 0) {
			perror("write");
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return -1;
		}
		w_end = rdtsc();
	} 

	if (strcmp(argv[2], "write") == 0) { 
		printf("%llu\n", w_end - w_start);
		close (pipe_fd[1]);
		exit(0);
	}
	set_prio(1);

	child = fork();
	if (child > 0) {
		close(pipe_fd[0]);
		usleep(U_WAIT);
		if (write_delay == 1 && write(pipe_fd[1], data_buf, strlen(data_buf)) < 0) {
			perror("write");
			close(pipe_fd[1]);
			exit (0);
		}

		close(pipe_fd[1]);
		wait(NULL);
		exit(0);
	} else if (child == 0) {
		char read_buf[PAGE_SIZE];

		set_prio(0);

		close(pipe_fd[1]);

		memset(read_buf, 0, PAGE_SIZE);

		r_start = rdtsc();
		if (read(pipe_fd[0], read_buf, PAGE_SIZE - 1) < 0) {
			perror("read");
			close(pipe_fd[0]);
			_exit(-1);
		}
		r_end = rdtsc();

		if (strncmp(argv[2], "read", 4) == 0) printf("%llu\n", r_end - r_start);

		close (pipe_fd[0]);
		_exit(0);
	} else {
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return -1;
	}

	return 0;
}

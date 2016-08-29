#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#define PAGE_SIZE   4096

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
	int fd;
	void *mapdata;
	unsigned long long start, end;

	if (argc != 3) {
		printf("Usage: %s <input_file> <mmap/munmap>\n", argv[0]);
		printf("\t <input_file> : file containing data for this test\n");
		printf("\t <mmap/munmap> : mmap - ubench for mmap\n\t\t munmap - ubench for munmap\n");
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	
	start = rdtsc();
	mapdata = mmap(NULL, PAGE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	if (mapdata == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
	end = rdtsc();

	if (strcmp(argv[2], "mmap") == 0) {
		printf("%llu\n", end - start);

		munmap(mapdata, PAGE_SIZE);
	} else if (strcmp(argv[2], "munmap") == 0) {
		start = rdtsc();
		munmap(mapdata, PAGE_SIZE);
		end = rdtsc();

		printf("%llu\n", end - start);
	}

	close(fd);

	return 0;
}

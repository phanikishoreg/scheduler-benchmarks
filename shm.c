#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NSEC_WAIT 10000000
#define SHM_NAME "/two-process-benchmark"
#define PAGE_SIZE (sizeof(unsigned long long))

pid_t parent;
pid_t child;
volatile unsigned long long *shm;
volatile unsigned long long end;

extern void set_prio (unsigned int);

static __inline__ unsigned long long
rdtsc (void)
{
  unsigned long long int x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
  return x;
}

void
handler (int sig, siginfo_t *si, void *v)
{
  end = rdtsc();
  printf ("%lld\n", end - *shm);
  kill (child, SIGKILL);
  wait (NULL);
  munmap ((void *)shm, PAGE_SIZE);
  shm_unlink (SHM_NAME);
  exit (0);
}

int
main (void)
{
  int shmfd;
  sigset_t ss;
  timer_t tid;
  struct itimerspec t;
  struct sigaction sa;

  parent = getpid ();

  shmfd = shm_open (SHM_NAME, O_RDWR | O_CREAT, 0644);
  if (shmfd < 0) {
    perror ("shm_open");
    return 1;
  }

  if (ftruncate (shmfd, PAGE_SIZE) < 0) {
    perror ("ftruncate");
    return 1;
  }

  shm = mmap (NULL, PAGE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shmfd, 0);
  if (shm == MAP_FAILED) {
    perror ("mmap");
    return 1;
  }

  sigfillset (&ss);
  sa.sa_handler = NULL;
  sa.sa_mask = ss;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_restorer = NULL;
  sa.sa_sigaction = handler;
  sigaction (SIGALRM, &sa, NULL);

  timer_create (CLOCK_REALTIME, NULL, &tid);
  t.it_interval.tv_sec = 0;
  t.it_interval.tv_nsec = 0;
  t.it_value.tv_sec = 0;
  t.it_value.tv_nsec = NSEC_WAIT;

  child = fork ();

  if (child > 0) {
    set_prio (0);
    timer_settime (tid, 0, &t, NULL);

    while (1)
      sleep (1);

  } else if (child == 0) {
    set_prio (1);
    while (1)
      *shm = rdtsc();

  } else {
    perror ("fork");
    exit (1);
  }

  return 1;
}




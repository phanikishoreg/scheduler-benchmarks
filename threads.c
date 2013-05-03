#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define NSEC_WAIT 1000000
#define SHM_NAME "/two-process-benchmark"
#define PAGE_SIZE (sizeof(unsigned long long))

pthread_t sleeper, spinner;
unsigned long long last, end;

extern void set_prio (unsigned int);
extern void pthread_prio (pthread_t, unsigned int);

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
  printf ("%lld\n", end - last);
  exit (0);
}

void *
sleep_thread (void *p)
{
  timer_t tid;
  struct itimerspec t;
  pthread_prio (sleeper, 0);
  timer_create (CLOCK_REALTIME, NULL, &tid);
  t.it_interval.tv_sec = 0;
  t.it_interval.tv_nsec = 0;
  t.it_value.tv_sec = 0;
  t.it_value.tv_nsec = NSEC_WAIT;
  timer_settime (tid, 0, &t, NULL);

  while (1)
    sleep (1);
}

void *
spin_thread (void *p)
{
  pthread_prio (spinner, 1);
  while (1)
    last = rdtsc();
}

int
main (void)
{
  sigset_t ss;
  struct sigaction sa;
  struct sched_param slparam, spparam;

  sigfillset (&ss);
  sa.sa_handler = NULL;
  sa.sa_mask = ss;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_restorer = NULL;
  sa.sa_sigaction = handler;
  sigaction (SIGALRM, &sa, NULL);

  set_prio (0);
  pthread_create (&sleeper, NULL, sleep_thread, NULL);
  pthread_create (&spinner, NULL, spin_thread, NULL);
  pthread_join (sleeper, NULL);
  pthread_join (spinner, NULL);

  return 1;
}

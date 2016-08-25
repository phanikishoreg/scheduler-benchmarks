#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#define NSEC_WAIT 1000

extern void set_prio (unsigned int);

unsigned long long last;
unsigned long long end;

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

int
main (void)
{
  sigset_t ss;
  timer_t tid;
  struct sigaction sa;
  struct itimerspec t;

  //setpriority (PRIO_PROCESS, 0, -20);
  set_prio(0);

  sigfillset (&ss);
  sa.sa_handler = NULL;
  sa.sa_sigaction = handler;
  sa.sa_mask = ss;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_restorer = NULL;
  sigaction (SIGALRM, &sa, NULL);

  timer_create (CLOCK_REALTIME, NULL, &tid);
  t.it_interval.tv_sec = 0;
  t.it_interval.tv_nsec = 0;
  t.it_value.tv_sec = 0;
  t.it_value.tv_nsec = NSEC_WAIT;

  last = rdtsc();

  timer_settime (tid, 0, &t, NULL);

  while (1)
    last = rdtsc();
  return 0;
}

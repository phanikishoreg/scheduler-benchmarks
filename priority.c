#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>

static void call_getrlimit(int id, char *name)
{
	struct rlimit rl;

	if (getrlimit(id, &rl)) {
		perror("getrlimit: ");
		exit(-1);
	}		
}

static void call_setrlimit(int id, rlim_t c, rlim_t m)
{
	struct rlimit rl;

	rl.rlim_cur = c;
	rl.rlim_max = m;
	if (setrlimit(id, &rl)) {
		perror("getrlimit: ");
		exit(-1);
	}		
}

void pthread_prio(pthread_t thread, unsigned int nice)
{
        struct sched_param sp;
	int policy;

        call_getrlimit(RLIMIT_CPU, "CPU");
#ifdef RLIMIT_RTTIME
        call_getrlimit(RLIMIT_RTTIME, "RTTIME");
#endif 
        call_getrlimit(RLIMIT_RTPRIO, "RTPRIO");
        call_setrlimit(RLIMIT_RTPRIO, RLIM_INFINITY, RLIM_INFINITY);
        call_getrlimit(RLIMIT_RTPRIO, "RTPRIO");
        call_getrlimit(RLIMIT_NICE, "NICE");

        if (pthread_getschedparam(thread, &policy, &sp) < 0) {
                perror("pth_getparam: ");
        }
        sp.sched_priority = sched_get_priority_max(SCHED_RR) - nice;
        if (pthread_setschedparam(thread, SCHED_RR, &sp) < 0) {
                perror("pth_setparam: ");
                exit(-1);
        }
        if (pthread_getschedparam(thread, &policy, &sp) < 0) {
                perror("getparam: ");
        }
        assert(sp.sched_priority == sched_get_priority_max(SCHED_RR) - nice);

        return;
}

void set_prio(unsigned int nice)
{
	struct sched_param sp;

	call_getrlimit(RLIMIT_CPU, "CPU");
#ifdef RLIMIT_RTTIME
	call_getrlimit(RLIMIT_RTTIME, "RTTIME");
#endif
	call_getrlimit(RLIMIT_RTPRIO, "RTPRIO");
	call_setrlimit(RLIMIT_RTPRIO, RLIM_INFINITY, RLIM_INFINITY);
	call_getrlimit(RLIMIT_RTPRIO, "RTPRIO");	
	call_getrlimit(RLIMIT_NICE, "NICE");

	if (sched_getparam(0, &sp) < 0) {
		perror("getparam: ");
	}
	sp.sched_priority = sched_get_priority_max(SCHED_RR) - nice;
	if (sched_setscheduler(0, SCHED_RR, &sp) < 0) {
		perror("setscheduler: ");
		exit(-1);
	}
	if (sched_getparam(0, &sp) < 0) {
		perror("getparam: ");
	}
	assert(sp.sched_priority == sched_get_priority_max(SCHED_RR) - nice);

	return;
}

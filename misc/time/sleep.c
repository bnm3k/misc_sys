#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

double to_secs(struct timespec* dur) {
    double s = (double)dur->tv_nsec / (double)1e9;
    return (double)dur->tv_sec + s;
}

void alarm_handler(int signum) {
    printf("alarm: %d\n", signum);
}

int main() {
    int ret = 0;

    // add signal handler
    signal(SIGALRM, alarm_handler);

    // set alarm
    struct itimerval dur    = {0};
    dur.it_value.tv_sec     = 3;
    dur.it_value.tv_usec    = 500000;
    dur.it_interval.tv_sec  = 0; // do not rearm
    dur.it_interval.tv_usec = 0; // do not rearm
    ret                     = setitimer(ITIMER_REAL, &dur, NULL);
    if (ret == -1) {
        perror("err: setitimer");
        exit(1);
    }

    // sleep
    time_t seconds   = 2;         // time_t is defined as long in linux
    long nanoseconds = 999999999; // 1 billion nanoseconds make a second, fyi
    struct timespec duration = {
        .tv_sec  = seconds,
        .tv_nsec = nanoseconds,
    };
    struct timespec remaining = {0};
    ret                       = nanosleep(&duration, &remaining);
    if (ret != 0) { // 0 is success
        // efault: req or rem is an invalid pointer
        // einval: one of the fields in req is invalid
        // eintr: interrupted by signal
        if (errno != EINTR) {
            perror("err: nanosleep");
            exit(1);
        }
    }
    double slept_for = to_secs(&duration) - to_secs(&remaining);
    printf("slept for %f/%f seconds\n", slept_for, to_secs(&duration));
    return 0;
}

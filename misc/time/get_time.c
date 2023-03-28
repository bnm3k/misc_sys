#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void err_exit(char* msg) {
    perror(msg);
    exit(1);
}

double to_sec(struct timespec* dur) {
    double s = (double)dur->tv_nsec / (double)1e9;
    return (double)dur->tv_sec + s;
}

int main() {

    int ret = 0;
    struct timespec start, stop;
    ret = clock_gettime(CLOCK_REALTIME, &start);
    if (ret == -1) err_exit("err: clock_gettime");

    sleep(1);

    ret = clock_gettime(CLOCK_REALTIME, &stop);
    if (ret == -1) err_exit("err: clock_gettime");

    printf("slept for: %f\n", to_sec(&stop) - to_sec(&start));
    return 0;
}

#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
    char* clock_as_str[] = {
        "CLOCK_REALTIME          ", "CLOCK_MONOTONIC         ",
        "CLOCK_MONOTONIC_RAW     ", "CLOCK_PROCESS_CPUTIME_ID",
        "CLOCK_THREAD_CPUTIME_ID "};

    clockid_t clocks[] = {CLOCK_REALTIME, CLOCK_MONOTONIC, CLOCK_MONOTONIC_RAW,
                          CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID};
    size_t clocks_num  = sizeof(clocks) / sizeof(clockid_t);
    struct timespec* res = calloc(sizeof(struct timespec), clocks_num);

    // get resolutions
    for (size_t i = 0; i < clocks_num; ++i) {
        int ret = clock_getres(clocks[i], res + i);
        if (ret != 0) perror("err: clock_getres");
    }

    // print resolutions
    for (size_t i = 0; i < clocks_num; ++i) {
        struct timespec* r = res + i;
        printf("%s sec=%ld nsec=%ld\n", clock_as_str[i], r->tv_sec, r->tv_nsec);
    }
    free(res);
    return 0;
}

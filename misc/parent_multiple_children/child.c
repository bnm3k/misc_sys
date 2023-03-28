#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct _command_line_args_t {
    time_t seconds_for_sleep;
    long nanoseconds_for_sleep;
} command_line_args_t;

command_line_args_t read_args(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    command_line_args_t args = read_args(argc, argv);

    // sleep
    struct timespec duration = {
        .tv_sec  = args.seconds_for_sleep,
        .tv_nsec = args.nanoseconds_for_sleep,
    };
    struct timespec remaining = {0};
    int ret                   = nanosleep(&duration, &remaining);
    if (ret != 0) { // 0 is success
        // efault: req or rem is an invalid pointer
        // einval: one of the fields in req is invalid
        // eintr: interrupted by signal
        if (errno != EINTR) {
            perror("err: nanosleep");
            return 1;
        }
    }
    return 0;
}

command_line_args_t read_args(int argc, char* argv[]) {
    char* usage_info_format =
        "Usage: %s -s <seconds> --ms <milliseconds> --us "
        "<microseconds> --ns <nanoseconds>\nNote: only set one of ms/us/ns";
    ssize_t usage_info_len = snprintf(NULL, 0, usage_info_format, argv[0]);
    char* usage_info       = malloc(usage_info_len + 1);
    snprintf(usage_info, usage_info_len + 1, usage_info_format, argv[0]);

    bool should_exit = false;
    int exit_status  = 0;

    // defaults
    command_line_args_t args;
    args.seconds_for_sleep = 1;
    bool nano_is_set       = false;

    int i = 1;
    while (i < argc) {
        char* arg = argv[i];
        if (strcmp(arg, "-s") == 0 || strcmp(arg, "--sec") == 0) {
            if (i + 1 < argc) {
                args.seconds_for_sleep = atol(argv[i + 1]);
                i += 2;
                continue;
            }
        } else if (strcmp(arg, "--ms") == 0 && !nano_is_set) {
            if (i + 1 < argc) {
                args.nanoseconds_for_sleep = atol(argv[i + 1]) * 1000000;
                nano_is_set                = true;
                i += 2;
                continue;
            }
        } else if (strcmp(arg, "--us") == 0 && !nano_is_set) {
            if (i + 1 < argc) {
                args.nanoseconds_for_sleep = atol(argv[i + 1]) * 1000;
                nano_is_set                = true;
                i += 2;
                continue;
            }
        } else if (strcmp(arg, "--ns") == 0 && !nano_is_set) {
            if (i + 1 < argc) {
                args.nanoseconds_for_sleep = atol(argv[i + 1]);
                nano_is_set                = true;
                i += 2;
                continue;
            }
        } else if (strcmp(arg, "-h") == 0) {
            printf("%s", usage_info);
            should_exit = true;
            exit_status = 0;
            break;
        }
        fprintf(stderr, "Error: Invalid arguments '%s'\n", arg);
        fprintf(stderr, "%s", usage_info);
        should_exit = true;
        exit_status = 1;
        break;
    }
    assert(args.nanoseconds_for_sleep < 1000000000);
    free(usage_info);
    if (should_exit) exit(exit_status);
    return args;
}

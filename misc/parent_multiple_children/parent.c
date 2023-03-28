#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void err_exit(char* msg) {
    perror(msg);
    exit(1);
}
int gen_rand_range(int min, int max_inclusive) {
    return (rand() % (max_inclusive - min + 1)) + min;
}

double to_sec(struct timespec* dur) {
    double s = (double)dur->tv_nsec / (double)1e9;
    return (double)dur->tv_sec + s;
}

typedef struct _command_line_args_t {
    int num_children;
    int min_time;
    int max_time;
} command_line_args_t;

command_line_args_t read_args(int argc, char* argv[]);

typedef struct _child_t {
    pid_t pid;
    struct timespec start;
    struct timespec stop;
    struct timespec duration_for_sleep;
    int status;
} child_t;

int main(int argc, char* argv[]) {
    srand(42); // TODO use current timestamp

    command_line_args_t args = read_args(argc, argv);
    child_t* children        = calloc(sizeof(child_t), args.num_children);

    // fork children
    for (size_t i = 0; i < (size_t)args.num_children; ++i) {
        // sleep params
        int seconds     = gen_rand_range(args.min_time, args.max_time - 1);
        int nanoseconds = gen_rand_range(0, 999999999);
        int res         = fork();
        switch (res) {
        case -1:
            perror("err: on fork");
            exit(1);
            break;
        case 0:; // at child
            char seconds_as_str[12]     = {0};
            char nanoseconds_as_str[12] = {0};
            sprintf(seconds_as_str, "%d", seconds);
            sprintf(nanoseconds_as_str, "%d", nanoseconds);
            char* args[] = {"./child",          "-s", seconds_as_str, "--ns",
                            nanoseconds_as_str, NULL};
            char* env[]  = {NULL};
            int ret      = execve(args[0], args, env); // env empty
            if (ret == -1) {
                perror("err: on execve");
                exit(1);
            }
            return 0; // shouldn't reach here
        default:;     // at parent
            child_t* c                    = children + i;
            c->pid                        = res;
            c->duration_for_sleep.tv_sec  = seconds;
            c->duration_for_sleep.tv_nsec = nanoseconds;

            // record start time
            ret = clock_gettime(CLOCK_REALTIME, &(c->start));
            if (ret == -1) err_exit("err: clock_gettime");
        }
    }

    // set deadline

    // wait for children
    int reaped = 0;
    while (reaped < args.num_children) {
        int status;
        pid_t pid = wait(&status);
        if (pid == -1) {
            perror("err: on wait"); // ECHILD, EINTR
            exit(1);
        }
        child_t* child = NULL;
        for (size_t i = 0; i < (size_t)args.num_children; ++i)
            if (children[i].pid == pid) {
                child = children + i;
                break;
            }
        if (child != NULL) {
            // record stop time
            int ret = clock_gettime(CLOCK_REALTIME, &(child->stop));
            if (ret == -1) err_exit("err: clock_gettime");
            child->status = status;
        } else {
            // should not be possible unless parent sired a child they did not
            // know about
            fprintf(stderr, "err: reaped child whose PID was not tracked\n");
            exit(1);
        }
        ++reaped;
    }

    // print stats
    for (size_t i = 0; i < (size_t)args.num_children; ++i) {
        child_t* c       = &children[i];
        double slept_for = to_sec(&(c->stop)) - to_sec(&(c->start));
        printf("child [%5d] slept for %.4f/%.4f", c->pid, slept_for,
               to_sec(&(c->duration_for_sleep)));
        if (WIFEXITED(c->status)) {
            printf(", normal termination with exit status: %d",
                   WEXITSTATUS(c->status));
        }
        if (WIFSIGNALED(c->status)) {
            printf(", killed by signal: %d %s", WTERMSIG(c->status),
                   WCOREDUMP(c->status) ? "(dumped core)" : "");
        }
        printf("\n");
    }
    free(children);
    return 0;
}

command_line_args_t read_args(int argc, char* argv[]) {
    char* usage_info_format = "Usage: %s\n\t-n <num_children>\n\t--min "
                              "<min_seconds>\n\t--max <max_seconds>\n";
    ssize_t usage_info_len = snprintf(NULL, 0, usage_info_format, argv[0]);
    char* usage_info       = malloc(usage_info_len + 1);
    snprintf(usage_info, usage_info_len + 1, usage_info_format, argv[0]);

    bool should_exit = false;
    int exit_status  = 0;

    command_line_args_t args;

    // defaults
    args.num_children = 5;
    args.min_time     = 1;
    args.max_time     = 5;

    int i = 1;
    while (i < argc) {
        char* arg = argv[i];
        if (strcmp(arg, "-n") == 0) {
            if (i + 1 < argc) {
                args.num_children = atoi(argv[i + 1]);
                i += 2;
                continue;
            }
        } else if (strcmp(arg, "--min") == 0) {
            if (i + 1 < argc) {
                args.min_time = atoi(argv[i + 1]);
                i += 2;
                continue;
            }
        } else if (strcmp(arg, "--max") == 0) {
            if (i + 1 < argc) {
                args.max_time = atoi(argv[i + 1]);
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
    free(usage_info);
    // verify args
    // min time should always be less than or equal to max
    assert(args.min_time <= args.max_time);
    // num_children should always be nonnegative
    assert(args.num_children >= 0);
    if (should_exit) exit(exit_status);
    return args;
}

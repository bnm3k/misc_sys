#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _command_line_args_t {
    int seconds_for_sleep;
} command_line_args_t;

command_line_args_t read_args(int argc, char* argv[]) {
    char* usage_info_format = "Usage: %s -t <seconds>\n";
    ssize_t usage_info_len  = snprintf(NULL, 0, usage_info_format, argv[0]);
    char* usage_info        = malloc(usage_info_len + 1);
    snprintf(usage_info, usage_info_len + 1, usage_info_format, argv[0]);

    bool should_exit = false;
    int exit_status  = 0;

    // defaults
    command_line_args_t args;
    args.seconds_for_sleep = 1;

    int i = 1;
    while (i < argc) {
        char* arg = argv[i];
        if (strcmp(arg, "-t") == 0) {
            if (i + 1 < argc) {
                args.seconds_for_sleep = atoi(argv[i + 1]);
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
    if (should_exit) exit(exit_status);
    return args;
}

int main(int argc, char* argv[]) {
    command_line_args_t args = read_args(argc, argv);
    if (args.seconds_for_sleep >= 0) sleep(args.seconds_for_sleep);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int ret = 0;
    ret     = fork();
    if (ret == -1) {
        perror("fork");
    } else if (ret == 0) {
        printf("[at child ] parent: %d\n", getppid());
        sleep(2);
        printf("\n[at child ] parent: %d\n", getppid());
        printf("[at child ] me:     %d\n", getpid());
        return 0;
    }
    printf("[at parent] parent: %d\n", getpid());
    return 0;
}

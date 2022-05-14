#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    int pipes[2];

    pid_t pEncoder, pDecoder;

    if (pipe(pipes)) {
        printf("Pipe error");
        exit(EXIT_FAILURE);
    }

    pDecoder = fork();

    switch (pDecoder) { 
        case(-1):
            printf("piper: couldn't fork\n");
            close(pipes[0]);
            close(pipes[1]);
            exit(EXIT_FAILURE);
        case(0):
            close(0);
            dup(pipes[0]);
            close(pipes[0]);
            close(pipes[1]);
            execl("./encod/enc", "./encod/enc", NULL);
            printf("decoder didn't starts\n");
            exit(EXIT_FAILURE);
        default:
            break;
    }

    pEncoder = fork();

    switch (pEncoder) {
        case(-1):
            printf("piper: couldn't fork\n");
            close(pipes[0]);
            close(pipes[1]);
            kill(pDecoder, 9);
            exit(EXIT_FAILURE);
        case(0):
            close(1);
            dup(pipes[1]);
            close(pipes[1]);
            close(pipes[0]);
            execl("./encod/enc", "./encod/enc", "r", NULL);
            printf("encoder didn't starts\n");
            exit(EXIT_FAILURE);
        default:
            break;
    }

    close(pipes[0]);
    close(pipes[1]);
    waitpid(pDecoder, NULL, 0);
    exit(EXIT_SUCCESS);
}
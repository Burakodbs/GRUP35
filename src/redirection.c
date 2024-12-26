/*

B221210073 - Berkay GÖÇER
B221210078 - Burak ODABAŞ
B211210021 - Buse Nur ÖĞÜNŞEN
B221210040 - Mert BAYIR
G221210073 - Elif GÜNAYDIN

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "redirection.h"

int setup_input_redirection(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Input redirection failed");
        return -1;
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("Input redirection failed");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int setup_output_redirection(const char *filename, int append) {
    int flags = O_WRONLY | O_CREAT;
    flags |= append ? O_APPEND : O_TRUNC;

    int fd = open(filename, flags, 0644);
    if (fd < 0) {
        perror("Output redirection failed");
        return -1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Output redirection failed");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "background.h"

#define MAX_BG_PROCESSES 64

static pid_t background_processes[MAX_BG_PROCESSES];
int process_count = 0;

void add_background_process(pid_t pid) {
    if (process_count < MAX_BG_PROCESSES) {
        background_processes[process_count++] = pid;
    }
}

void monitor_background_processes() {
    int status;
    pid_t pid;

    for (int i = 0; i < process_count; i++) {
        pid = waitpid(background_processes[i], &status, WNOHANG);
        if (pid > 0) {
            printf("[%d] retval: %d\n", pid, WEXITSTATUS(status));

            // Remove from list
            for (int j = i; j < process_count - 1; j++) {
                background_processes[j] = background_processes[j + 1];
            }
            process_count--;
            i--;  // Recheck this index
        }
    }
}
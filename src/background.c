/**
* @file           background.c
* @description    İşletim Sistemleri Dersi Proje Ödevi
* @course         1A ve 2A grubu
* @assignment     Projeodevi
* @date           26.12.2024
* @author         Elif Günaydın elif.gunaydin2@ogr.sakarya.edu.tr
* @author         Mert Bayır mert.bayir1@ogr.sakarya.edu.tr
* @author         Buse Nur Öğünşen buse.ogunsen@ogr.sakarya.edu.tr
* @author         Berkay Göçer berkay.gocer@ogr.sakarya.edu.tr
* @author         Burak Odabaş burak.odabas2@ogr.sakarya.edu.tr
*/

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
/**
* @file           redirection.c
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
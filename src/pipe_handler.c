/**
* @file           pipe_handler.c
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
#include <string.h>
#include <sys/wait.h>
#include "pipe_handler.h"
#include <command_handler.h>
#include "utils.h"


void execute_pipe(char **commands) {
    int num_commands = 0;
    while (commands[num_commands] != NULL) num_commands++;

    int pipes[MAX_COMMANDS][2];
    pid_t pids[MAX_COMMANDS];

    for (int i = 0; i < num_commands; i++) {
        if (i < num_commands - 1) {
            if (pipe(pipes[i]) < 0) {
                perror("Pipe creation failed");
                return;
            }
        }

        pids[i] = fork();
        if (pids[i] < 0) {
            perror("Fork failed");
            return;
        }

        if (pids[i] == 0) {
            // Çocuk process

            // Önceki komutun çıkışını bu komutun girişine bağla
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Bu komutun çıkışını sonraki komutun girişine bağla
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Kullanılmayan pipe'ları kapat
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Komutu çalıştır
            char *cmd = trim(commands[i]);
            char *args[MAX_ARGS];
            tokenize_command(cmd, args, MAX_ARGS);
            execvp(args[0], args);

            perror("Pipe command execution failed");
            exit(1);
        }

        // Ana process: Önceki pipe'ı kapat
        if (i > 0) {
            close(pipes[i-1][0]);
            close(pipes[i-1][1]);
        }
    }

    // Ana process: Tüm child process'leri bekle
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
}
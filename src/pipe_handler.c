/**
* @file           pipe_handler.c
* @description    İşletim Sistemleri Dersi Proje Ödevi
* @course         1A ,1C ve 2A grubu
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

/**
 * Pipe ile bağlı komutları çalıştırır
 * @param commands Çalıştırılacak komutlar dizisi
 */
void execute_pipe(char **commands) {
    // Toplam komut sayısını hesapla
    int num_commands = 0;
    while (commands[num_commands] != NULL) num_commands++;

    // Her komut çifti için pipe ve process ID'leri için diziler
    int pipes[MAX_COMMANDS][2];
    pid_t pids[MAX_COMMANDS];

    // Her komut için döngü
    for (int i = 0; i < num_commands; i++) {
        // Son komut hariç her komut için yeni pipe oluştur
        if (i < num_commands - 1) {
            if (pipe(pipes[i]) < 0) {
                perror("Pipe creation failed");
                return;
            }
        }

        // Her komut için yeni process oluştur
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("Fork failed");
            return;
        }

        if (pids[i] == 0) {
            // Çocuk process: pipe bağlantılarını ayarla

            // İlk komut değilse, önceki komutun çıktısını bu komutun girişine bağla
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Son komut değilse, bu komutun çıktısını sonraki komutun girişine bağla
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Kullanılmayan tüm pipe uçlarını kapat
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Komutu argümanlara ayır ve çalıştır
            char *cmd = trim(commands[i]);
            char *args[MAX_ARGS];
            tokenize_command(cmd, args, MAX_ARGS);
            execvp(args[0], args);

            // Komut çalıştırılamazsa hata ver
            perror("Pipe command execution failed");
            exit(1);
        }

        // Ana process: kullanılmayan pipe uçlarını kapat
        if (i > 0) {
            close(pipes[i-1][0]);
            close(pipes[i-1][1]);
        }
    }

    // Ana process: tüm çocuk process'lerin tamamlanmasını bekle
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
}
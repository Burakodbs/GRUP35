/**
* @file           main.c
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
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "command_handler.h"
#include "background.h"
#include "utils.h"

// Maksimum uzunluk sabitleri
#define MAX_HOSTNAME 1024
#define MAX_CWD 1024

// Shell'in çalışma durumunu tutan değişken
static volatile int running = 1;

/**
 * Shell komut istemini ekrana yazdırır
 */
void print_prompt() {
    printf("> ");
    fflush(stdout);
}

/**
 * Sinyal işleyici fonksiyonu
 * SIGINT (Ctrl+C) sinyalini yakalar ve uygun şekilde işler
 * @param signo Yakalanan sinyalin numarası
 */
void signal_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n");
        print_prompt();
        fflush(stdout);
    }
}

/**
 * Ana program döngüsü
 */
int main() {
    char *command = NULL;
    size_t bufsize = 0;

    // SIGINT sinyali için işleyici tanımla
    signal(SIGINT, signal_handler);

    // Komut geçmişini dosyadan oku
    load_history();

    // Ana shell döngüsü
    while (running) {
        print_prompt();

        // Kullanıcıdan komut oku
        ssize_t characters = getline(&command, &bufsize, stdin);
        if (characters == -1) {
            break;
        }

        // Satır sonu karakterini kaldır
        if (command[characters - 1] == '\n') {
            command[characters - 1] = '\0';
        }

        // Boş komutları atla
        if (strlen(command) == 0) {
            continue;
        }

        // Komutu geçmişe ekle
        add_to_history(command);

        // Çıkış komutlarını kontrol et
        if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
            // Tüm arka plan işlemlerinin tamamlanmasını bekle
            while (1) {
                monitor_background_processes();
                extern int process_count;
                if (process_count == 0) {
                    break;
                }
                usleep(100000);  // CPU kullanımını azaltmak için bekle
            }
            break;
        }

        // Komutu işle ve çalıştır
        handle_command(command);
    }

    // Program sonlandırma işlemleri
    save_history();
    free(command);
    return 0;
}
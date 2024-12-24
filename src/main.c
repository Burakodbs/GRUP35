#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "command_handler.h"
#include "background.h"
#include "utils.h"

#define MAX_HOSTNAME 1024
#define MAX_CWD 1024

static volatile int running = 1;

void print_prompt() {
    printf("> ");
    fflush(stdout);
}

void signal_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n");
        print_prompt();
        fflush(stdout);
    }
}



int main() {
    char *command = NULL;
    size_t bufsize = 0;

    // Signal handler kurulumu
    signal(SIGINT, signal_handler);

    // History dosyasını yükle
    load_history();

    while (running) {
        print_prompt();

        ssize_t characters = getline(&command, &bufsize, stdin);
        if (characters == -1) {
            break;
        }

        // Satır sonunu kaldır
        if (command[characters - 1] == '\n') {
            command[characters - 1] = '\0';
        }

        // Boş komutları atla
        if (strlen(command) == 0) {
            continue;
        }

        // Komutu history'e ekle
        add_to_history(command);

        // "exit" veya "quit" kontrolü
        if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
            // Tüm arka plan işlemleri bitene kadar bekle
            while (1) {
                // monitor_background_processes çağrısı bir arka plan işlemi
                // tamamlandığında bunu raporlayacak
                monitor_background_processes();

                // process_count background.c'de tanımlı ve mevcut arka plan işlem sayısını tutuyor
                extern int process_count;  // background.c'den process_count'u al
                if (process_count == 0) {
                    break;  // Tüm arka plan işlemleri bitti
                }
                usleep(100000);  // 100ms bekle, CPU'yu yormamak için
            }
            break;
        }

        // Komutu işle
        handle_command(command);
    }

    // Cleanup
    save_history();
    free(command);
    return 0;
}
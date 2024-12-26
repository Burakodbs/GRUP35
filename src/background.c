/**
* @file           background.c
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
#include <sys/wait.h>
#include "background.h"

// Arka planda çalışan maksimum process sayısını tanımlar
#define MAX_BG_PROCESSES 64

// Arka plan işlemlerinin PID'lerini tutan dizi
static pid_t background_processes[MAX_BG_PROCESSES];
// Mevcut arka plan işlem sayısını tutan sayaç
int process_count = 0;

/**
 * Yeni bir arka plan işlemini listeye ekler
 * @param pid Eklenecek işlemin process ID'si
 */
void add_background_process(pid_t pid) {
    // Maksimum process sayısı aşılmadıysa yeni process'i ekle
    if (process_count < MAX_BG_PROCESSES) {
        background_processes[process_count++] = pid;
    }
}

/**
 * Arka planda çalışan işlemleri kontrol eder
 * Tamamlanan işlemlerin çıkış durumlarını yazdırır ve listeden kaldırır
 */
void monitor_background_processes() {
    int status;
    pid_t pid;

    // Tüm arka plan işlemlerini kontrol et
    for (int i = 0; i < process_count; i++) {
        // WNOHANG ile process'in durumunu bloklamadan kontrol et
        pid = waitpid(background_processes[i], &status, WNOHANG);
        if (pid > 0) {
            // Tamamlanan process'in PID ve çıkış değerini yazdır
            printf("[%d] retval: %d\n", pid, WEXITSTATUS(status));

            // Tamamlanan process'i listeden kaldır
            for (int j = i; j < process_count - 1; j++) {
                background_processes[j] = background_processes[j + 1];
            }
            process_count--;
            i--;  // Kaldırılan elemandan sonraki indeksi tekrar kontrol et
        }
    }
}
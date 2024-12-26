/**
* @file           redirection.c
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
#include <fcntl.h>
#include "redirection.h"

/**
 * Girdi yönlendirmesini ayarlar
 * @param filename Girdi dosyasının adı
 * @return Başarılıysa 0, hata durumunda -1
 */
int setup_input_redirection(const char *filename) {
    // Dosyayı sadece okuma modunda aç
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Input redirection failed");
        return -1;
    }

    // Standart girdiyi dosyaya yönlendir
    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("Input redirection failed");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

/**
 * Çıktı yönlendirmesini ayarlar
 * @param filename Çıktı dosyasının adı
 * @param append Ekleme modu (1) veya üzerine yazma modu (0)
 * @return Başarılıysa 0, hata durumunda -1
 */
int setup_output_redirection(const char *filename, int append) {
    // Dosya açma bayraklarını ayarla (yazma, oluşturma ve ekleme/üzerine yazma)
    int flags = O_WRONLY | O_CREAT;
    flags |= append ? O_APPEND : O_TRUNC;

    // Dosyayı belirtilen modda aç
    int fd = open(filename, flags, 0644);
    if (fd < 0) {
        perror("Output redirection failed");
        return -1;
    }

    // Standart çıktıyı dosyaya yönlendir
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Output redirection failed");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}
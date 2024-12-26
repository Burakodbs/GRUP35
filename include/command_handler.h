/**
* @file           command_handler.h
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

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#define MAX_ARGS 64
#define MAX_COMMANDS 32

typedef struct {
    char *input_file;
    char *output_file;
    int append_output;
    int background;
} IOFlags;

void handle_command(char *command);

#endif
/**
* @file           utils.h
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

#ifndef UTILS_H
#define UTILS_H

// String işleme fonksiyonları
char *trim(char *str);
int tokenize_command(char *command, char **args, int max_args);
char **split_string(const char *input, const char *delimiter);
void free_split_string(char **split);

// History yönetimi
void add_to_history(const char *command);
void load_history(void);
void save_history(void);

#endif
/**
* @file           utils.c
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
#include <ctype.h>
#include "utils.h"

// Komut geçmişi için sabitler
#define HISTORY_FILE ".shell_history"
#define MAX_HISTORY 1000

// Komut geçmişi için statik değişkenler
static char *history[MAX_HISTORY];
static int history_count = 0;

/**
 * Verilen dizginin başındaki ve sonundaki boşlukları temizler
 * @param str Temizlenecek dizgi
 * @return Temizlenmiş dizgi
 */
char *trim(char *str) {
    if (!str) return NULL;

    // Baştaki boşlukları atla
    while (isspace(*str)) str++;

    if (*str == 0) return str;

    // Sondaki boşlukları temizle
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;

    end[1] = '\0';
    return str;
}

/**
 * Komutu boşluk ve tab karakterlerine göre parçalara ayırır
 * @param command Ayrıştırılacak komut
 * @param args Argümanların saklanacağı dizi
 * @param max_args Maksimum argüman sayısı
 * @return Argüman sayısı
 */
int tokenize_command(char *command, char **args, int max_args) {
    int arg_count = 0;
    char *token = strtok(command, " \t");

    while (token != NULL && arg_count < max_args - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t");
    }

    args[arg_count] = NULL;
    return arg_count;
}

/**
 * Dizgiyi belirtilen ayırıcıya göre parçalara böler
 * @param input Bölünecek dizgi
 * @param delimiter Ayırıcı karakter(ler)
 * @return Bölünmüş parçaları içeren karakter dizisi
 */
char **split_string(const char *input, const char *delimiter) {
    if (!input || !delimiter) return NULL;

    char *str = strdup(input);
    if (!str) return NULL;

    int count = 0;
    char **result = NULL;
    char *token = strtok(str, delimiter);

    while (token) {
        // Bellek alanını genişlet
        char **temp = realloc(result, sizeof(char*) * (count + 1));
        if (!temp) {
            free_split_string(result);
            free(str);
            return NULL;
        }
        result = temp;

        // Yeni parçayı kopyala
        result[count] = strdup(token);
        if (!result[count]) {
            free_split_string(result);
            free(str);
            return NULL;
        }

        count++;
        token = strtok(NULL, delimiter);
    }

    // NULL sonlandırıcı için yer ekle
    char **temp = realloc(result, sizeof(char*) * (count + 1));
    if (!temp) {
        free_split_string(result);
        free(str);
        return NULL;
    }
    result = temp;
    result[count] = NULL;

    free(str);
    return result;
}

/**
 * split_string ile ayrıştırılan belleği temizler
 * @param split Temizlenecek karakter dizisi
 */
void free_split_string(char **split) {
    if (!split) return;

    for (int i = 0; split[i]; i++) {
        free(split[i]);
    }
    free(split);
}

/**
 * Komutu geçmiş listesine ekler
 * @param command Eklenecek komut
 */
void add_to_history(const char *command) {
    if (history_count >= MAX_HISTORY) {
        // Liste doluysa en eski komutu sil
        free(history[0]);
        for (int i = 1; i < MAX_HISTORY; i++) {
            history[i-1] = history[i];
        }
        history_count--;
    }

    history[history_count++] = strdup(command);
}

/**
 * Komut geçmişini dosyadan yükler
 */
void load_history() {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", getenv("HOME"), HISTORY_FILE);

    FILE *fp = fopen(path, "r");
    if (!fp) return;

    char line[1024];
    while (fgets(line, sizeof(line), fp) && history_count < MAX_HISTORY) {
        line[strcspn(line, "\n")] = 0;
        add_to_history(line);
    }

    fclose(fp);
}

/**
 * Komut geçmişini dosyaya kaydeder
 */
void save_history() {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", getenv("HOME"), HISTORY_FILE);

    FILE *fp = fopen(path, "w");
    if (!fp) return;

    for (int i = 0; i < history_count; i++) {
        fprintf(fp, "%s\n", history[i]);
    }

    fclose(fp);
}
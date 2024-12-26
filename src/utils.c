/*

B221210073 - Berkay GÖÇER
B221210078 - Burak ODABAŞ
B211210021 - Buse Nur ÖĞÜNŞEN
B221210040 - Mert BAYIR
G221210073 - Elif GÜNAYDIN

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define HISTORY_FILE ".shell_history"
#define MAX_HISTORY 1000

static char *history[MAX_HISTORY];
static int history_count = 0;

char *trim(char *str) {
    if (!str) return NULL;

    // Trim leading spaces
    while (isspace(*str)) str++;

    if (*str == 0) return str;

    // Trim trailing spaces
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;

    end[1] = '\0';
    return str;
}

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

char **split_string(const char *input, const char *delimiter) {
    if (!input || !delimiter) return NULL;

    char *str = strdup(input);
    if (!str) return NULL;

    int count = 0;
    char **result = NULL;
    char *token = strtok(str, delimiter);

    while (token) {
        char **temp = realloc(result, sizeof(char*) * (count + 1));
        if (!temp) {
            free_split_string(result);
            free(str);
            return NULL;
        }
        result = temp;

        result[count] = strdup(token);
        if (!result[count]) {
            free_split_string(result);
            free(str);
            return NULL;
        }

        count++;
        token = strtok(NULL, delimiter);
    }

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

void free_split_string(char **split) {
    if (!split) return;

    for (int i = 0; split[i]; i++) {
        free(split[i]);
    }
    free(split);
}

void add_to_history(const char *command) {
    if (history_count >= MAX_HISTORY) {
        free(history[0]);
        for (int i = 1; i < MAX_HISTORY; i++) {
            history[i-1] = history[i];
        }
        history_count--;
    }

    history[history_count++] = strdup(command);
}

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
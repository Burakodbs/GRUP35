/*

B221210073 - Berkay GÖÇER
B221210078 - Burak ODABAŞ
B211210021 - Buse Nur ÖĞÜNŞEN
B221210040 - Mert BAYIR
G221210073 - Elif GÜNAYDIN

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
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
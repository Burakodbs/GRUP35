/**
* @file           command_handler.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "command_handler.h"
#include "redirection.h"
#include "pipe_handler.h"
#include "background.h"
#include "utils.h"


static IOFlags parse_io_flags(char *command) {
    IOFlags flags = {NULL, NULL, 0, 0};

    // Background check
    if (command[strlen(command) - 1] == '&') {
        flags.background = 1;
        command[strlen(command) - 1] = '\0';
        // Remove trailing spaces
        while (strlen(command) > 0 && command[strlen(command) - 1] == ' ') {
            command[strlen(command) - 1] = '\0';
        }
    }

    // Input redirection
    char *input = strstr(command, "<");
    if (input) {
        *input = '\0';
        flags.input_file = trim(input + 1);
    }

    // Output redirection with append
    char *output = strstr(command, ">>");
    if (output) {
        *output = '\0';
        flags.output_file = trim(output + 2);
        flags.append_output = 1;
    } else {
        // Normal output redirection
        output = strstr(command, ">");
        if (output) {
            *output = '\0';
            flags.output_file = trim(output + 1);
        }
    }

    return flags;
}

static int handle_builtin_echo(char **args) {
    int i = 1;
    while (args[i] != NULL) {
        printf("%s", args[i]);
        if (args[i + 1] != NULL) {
            printf(" ");
        }
        i++;
    }
    printf("\n");
    return 1;
}

static int handle_builtin_commands(char **args) {
    if (!args[0]) return 0;

    if (strcmp(args[0], "cd") == 0) {
        if (!args[1]) {
            const char *home = getenv("HOME");
            if (home) chdir(home);
            else fprintf(stderr, "cd: HOME not set\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
        }
        return 1;
    }

    if (strcmp(args[0], "echo") == 0) {
        return handle_builtin_echo(args);
    }

    return 0;
}

static void execute_single_command(char *command, IOFlags *flags) {
    char *args[MAX_ARGS];
    int arg_count = tokenize_command(command, args, MAX_ARGS);

    if (arg_count == 0) return;

    // Built-in komutlar için özel işlem
    if (!flags->input_file && !flags->output_file && handle_builtin_commands(args)) {
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Çocuk process

        // I/O yönlendirmelerini ayarla
        if (flags->input_file) {
            int fd = open(flags->input_file, O_RDONLY);
            if (fd == -1) {
                perror("Input redirection failed");
                exit(1);
            }
            if (dup2(fd, STDIN_FILENO) == -1) {
                perror("Input redirection dup2 failed");
                exit(1);
            }
            close(fd);
        }

        if (flags->output_file) {
            int flags_out = O_WRONLY | O_CREAT;
            flags_out |= flags->append_output ? O_APPEND : O_TRUNC;

            int fd = open(flags->output_file, flags_out, 0644);
            if (fd == -1) {
                perror("Output redirection failed");
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("Output redirection dup2 failed");
                exit(1);
            }
            close(fd);
        }

        // Tüm açık dosya tanımlayıcılarını temizle
        for (int i = 3; i < 256; i++) {
            close(i);
        }

        execvp(args[0], args);
        perror("Command execution failed");
        exit(1);
    } else if (pid > 0) {
        // Ana process
        if (!flags->background) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                // Komut hata ile sonlandı
                return;
            }
        } else {
            printf("[%d] Background process started\n", pid);
            add_background_process(pid);
        }
    } else {
        perror("Fork failed");
    }
}

static void handle_sequential_commands(char *command_line) {
    char **commands = split_string(command_line, ";");
    if (!commands) return;

    for (int i = 0; commands[i] != NULL; i++) {
        char *cmd = trim(commands[i]);
        if (strlen(cmd) == 0) continue;

        // Her komut için I/O flags'leri parse et
        char *cmd_copy = strdup(cmd);
        IOFlags flags = parse_io_flags(cmd_copy);

        if (strchr(cmd, '|')) {
            // Pipe varsa
            char **pipe_commands = split_string(cmd, "|");
            execute_pipe(pipe_commands);
            free_split_string(pipe_commands);
        } else {
            // Tek komut
            execute_single_command(cmd_copy, &flags);
        }

        free(cmd_copy);
    }

    free_split_string(commands);
}

void handle_command(char *command) {
    if (!command || strlen(command) == 0) return;

    if (strchr(command, ';')) {
        handle_sequential_commands(command);
        return;
    }

    // Komutun kopyasını al
    char *cmd_copy = strdup(command);
    IOFlags flags = parse_io_flags(cmd_copy);

    if (strchr(cmd_copy, '|')) {
        char **pipe_commands = split_string(cmd_copy, "|");
        execute_pipe(pipe_commands);
        free_split_string(pipe_commands);
    } else {
        execute_single_command(cmd_copy, &flags);
    }

    free(cmd_copy);
}
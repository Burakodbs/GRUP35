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
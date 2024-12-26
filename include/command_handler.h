/*

B221210073 - Berkay GÖÇER
B221210078 - Burak ODABAŞ
B211210021 - Buse Nur ÖĞÜNŞEN
B221210040 - Mert BAYIR
G221210073 - Elif GÜNAYDIN

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
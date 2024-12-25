#ifndef REDIRECTION_H
#define REDIRECTION_H

int setup_input_redirection(const char *filename);
int setup_output_redirection(const char *filename, int append);

#endif
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <sys/types.h>

extern int process_count;
void add_background_process(pid_t pid);
void monitor_background_processes(void);

#endif
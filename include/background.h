/*

B221210073 - Berkay GÖÇER
B221210078 - Burak ODABAŞ
B211210021 - Buse Nur ÖĞÜNŞEN
B221210040 - Mert BAYIR
G221210073 - Elif GÜNAYDIN

*/
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <sys/types.h>

extern int process_count;
void add_background_process(pid_t pid);
void monitor_background_processes(void);

#endif
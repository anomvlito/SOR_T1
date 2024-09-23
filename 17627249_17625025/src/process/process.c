#include "process.h"

Process* create_process(char name, int pid, estado_t estado, int burst_time, int num_bursts, int io_wait_time, int deadline) 
{
    Process* process = calloc(1, sizeof(Process));
    strncpy(process->nombre, name, sizeof(process->nombre) - 1);
    process->nombre[sizeof(process->nombre) - 1] = '\0';
    process->pid = pid;
    process->estado = estado;
    process->burst_time = burst_time;
    process->num_bursts = num_bursts;
    process->io_wait_time = io_wait_time;
    process->deadline = deadline;
    return process;
};


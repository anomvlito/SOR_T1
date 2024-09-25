#include "process.h"

Process *create_process(char *name, int pid, int t_inicio, estado_t estado,
                        int burst_time, int num_bursts, int io_wait_time,
                        int deadline) {
  Process *process = calloc(1, sizeof(Process));
  strncpy(process->nombre, name, sizeof(process->nombre) - 1);
  process->nombre[sizeof(process->nombre) - 1] = '\0';
  process->pid = pid;
  process->t_inicio = t_inicio;
  process->estado = READY;
  process->ultima_cola_visitada = HIGH_QUEUE;
  process->burst_time = burst_time;
  process->num_bursts_solicitados_por_proceso = num_bursts;
  process->io_wait_time = io_wait_time;
  process->deadline = deadline;
  process->quantum = 0;
  process->interrupciones = 0;
  process->turnaround_time = 0;
  process->response_time = 0;
  process->waiting_time = 0;
  process->deadline_sum = 0;
  process->num_current_complete_burst = 0;
  process->current_burst = 0;
  process->current_io_wait_time = 0;
  process->t_LCPU = 0;
  return process;
}

#pragma once

// Definir el struct Process:

// Variables básicas: Asegúrate de incluir todas las variables mencionadas en el
// enunciado: Nombre (char name[...];) PID (int pid;) Estado (enum {RUNNING,
// READY, WAITING, FINISHED} status;) Tiempo de ejecución por ráfaga (int
// burst_time;) Número de ráfagas de CPU (int num_bursts;) Tiempo de espera para
// I/O entre ráfagas (int io_wait_time;) Deadline de ejecución (int deadline;)
// Variables adicionales: Podrías necesitar variables adicionales para llevar el
// control, como: Tiempo de inicio (int start_time;) Tiempo en que salió por
// última vez de la CPU (int last_cpu_time;) Quantum restante (int
// remaining_quantum;) Contadores para estadísticas (interrupciones, tiempos de
// espera, etc.)

typedef enum {RUNNING, READY, WAITING, FINISHED} estado_t;

typedef struct process {
  char nombre[10];
  int pid;
  estado_t estado;
  int burst_time; // Tiempo de ejecucion por rafaga
  int num_bursts; //
  int io_wait_time;
  int deadline;
} Process;

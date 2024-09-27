#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

typedef enum { RUNNING, READY, WAITING, FINISHED } estado_t;
typedef enum { HIGH_QUEUE, LOW_QUEUE } ultima_cola_visitada_t;

typedef struct process {
  char nombre[10];
  int pid;
  int t_inicio;
  int burst_time;                         // Tiempo de ejecucion por rafaga
  int num_bursts_solicitados_por_proceso; //
  int io_wait_time;
  int deadline;
  estado_t estado;
  ultima_cola_visitada_t ultima_cola_visitada;
  int quantum;
  int t_LCPU;
  char queue[10];
  int interrupciones;
  int turnaround_time;
  int response_time;
  int waiting_time; // numero de veces que se encuentre en estado waiting y
                    // ready (sin contar ready cuando pasa a running)
  // sumar +1 siempre que este en ready y waiting, al ingresar un proceso a la
  // cpu ANTES de actualizar su estado a running, reviar si es ready, si es asi
  // hay que restarle 1.
  int deadline_sum;
  int num_current_complete_burst; // para saber si es su primera rafaga, y su
                                  // ultima para pasar a finished
  int current_burst;              // para saber si sale de la CPU
  int current_io_wait_time; // para saber cuando pueda pasar a READY, en cada
                            // iteracion que esta en waiting se le suma 1 y al
  // igualarse con io_wait_time queda en ready

} Process;

typedef struct node {
  Process *process;
  struct node *next;
  struct node *previous;
} Node;

typedef struct queue {
  Node *first_process;
  Node *last_process;
  int len;
  int quantum;
} Queue;

// Funciones declaracion

// process.h
Process *create_process(char **line);

Queue *create_queue(int capacity, int quantum);
Node *create_node(Process *process);

void add_node(Queue *queue, Node *node);

Process *extract_process(Queue *queue, Process *process);

Process *extraer_prioritario(Queue *queue, int tick);

void queue_free(Queue *queue);

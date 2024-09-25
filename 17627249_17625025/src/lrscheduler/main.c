#include "../file_manager/manager.h"
#include "../finished_processes/finished_processes.h"
#include "../process/process.h"
#include "../queue/queue.h"

#include <stdbool.h> // bool, true, false
#include <stdio.h>   // FILE, fopen, fclose, etc.
#include <stdlib.h>  // malloc, calloc, free, etc
#include <string.h>  // strtok, strcpy, etc.

void imprime_inicio(char *file_name, InputFile *input_file) {
  /*Mostramos el archivo de input en consola*/
  printf("Nombre archivo: %s\n", file_name);
  printf("Cantidad de procesos: %d\n", input_file->len);
  printf("Procesos:\n");
  for (int i = 0; i < input_file->len; ++i) {
    for (int j = 0; j < 7; ++j) {
      printf("%s ", input_file->lines[i][j]);
    }
    printf("\n");
  }
}

void update_waiting_processes(Queue *queue, int Tactual) {
  for (int i = 0; i < queue->capacity; i++) {
    Node *node = queue->nodes[i];
    Process *actual_pivot = get_process_by_id(node->id);
    if (actual_pivot->estado == WAITING &&
        (actual_pivot->current_io_wait_time > 0)) {
      actual_pivot->current_io_wait_time--;
      if (actual_pivot->current_io_wait_time == 0) {
        actual_pivot->estado = READY;
        // Recalcular la prioridad
        int new_priority =
            (Tactual - actual_pivot->t_LCPU) - actual_pivot->deadline;
        node->priority = new_priority;
        // Re-heapificar desde el índice actual
        max_percolate_up(queue, i);
        max_percolate_down(queue, i);
      }
    }
  }
}

//
void move_processes_from_low_to_high(Queue *low_queue, Queue *high_queue,
                                     int tick) {}

// Función para actualizar el estado de los procesos que han terminado su tiempo
// de espera de I/O
void update_waiting_processes(Queue *queue, int tick) {
  // for (int i = 0; i < queue->capacity; i++) {
  //   Node *node = queue->nodes[i];
  //   Process *process = get_process_by_id(node->id);
  //   if (process->estado == WAITING && process->current_io_wait_time > 0) {
  //     process->current_io_wait_time--;
  //     if (process->current_io_wait_time == 0) {
  //       process->estado = READY;
  //       // Recalcular la prioridad
  //       int new_priority = (tick - process->t_LCPU) - process->deadline;
  //       node->priority = new_priority;
  //       // Re-heapificar desde el índice actual
  //       max_percolate_up(queue, i);
  //       max_percolate_down(queue, i);
  //     }
  //   }
  // }
}

///
Process *priority_process(Queue *high_queue, Queue *low_queue) {}

void scheduler(Queue *high_queue, Queue *low_queue, Process **cpu_process,
               int tick, FinishedProcessList *finished_list,
               int *procesos_restantes, int *Numero_de_procesos,
               Process **processes) {

  // 1) Actualizar los procesos que hayan terminado su tiempo de espera de I/O
  // de `WAITING` a `READY`
  update_waiting_processes(high_queue, tick);
  update_waiting_processes(low_queue, tick);

  for (int i = 0; i < high_queue->size; i++) {
    Process *process = high_queue->nodes[i]->process;
    if (process->estado == READY) {
      process->waiting_time++; // Incrementar el waiting time
    }
  }

  for (int i = 0; i < low_queue->size; i++) {
    Process *process = low_queue->nodes[i]->process;
    if (process->estado == READY) {
      process->waiting_time++; // Incrementar el waiting time
    }
  }

  // 2) Si hay un proceso en estado `RUNNING`, actualizar su estado según
  // corresponda
  if (*cpu_process != NULL) {
    (*cpu_process)->quantum--;
    (*cpu_process)->current_burst++;

    // Caso 2a: Si no queda quantum
    if ((*cpu_process)->quantum == 0) {
      if ((*cpu_process)->burst_time - (*cpu_process)->current_burst > 0) {
        // La ráfaga no ha terminado
        (*cpu_process)->interrupciones++; // Aumentar interrupciones
        (*cpu_process)->estado = READY;   // Proceso sigue en estado READY
        (*cpu_process)->quantum =
            (*cpu_process)->ultima_cola_visitada == HIGH_QUEUE ? 2 * q : q;
        (*cpu_process)->t_LCPU = tick;
        queue_insert_max((*cpu_process)->ultima_cola_visitada == HIGH_QUEUE
                             ? high_queue
                             : low_queue,
                         *cpu_process, (*cpu_process)->priority);
      } else {
        // La ráfaga ha terminado
        (*cpu_process)->num_current_complete_burst++;
        if ((*cpu_process)->num_bursts_solicitados_por_proceso -
                (*cpu_process)->num_current_complete_burst >
            0) {
          // Aún quedan ráfagas por ejecutar
          (*cpu_process)->estado = WAITING;
          (*cpu_process)->quantum = quantum;
          (*cpu_process)->current_io_wait_time = (*cpu_process)->io_wait_time;
          (*cpu_process)->t_LCPU = tick;
          queue_insert_max(low_queue, *cpu_process, (*cpu_process)->priority);
        } else {
          // El proceso termina su ejecución
          (*cpu_process)->estado = FINISHED;
          *procesos_restantes -= 1;
          (*cpu_process)->t_LCPU = tick;
          add_finished_process(finished_list, *cpu_process);
        }
      }
      *cpu_process = NULL;
    } else {
      // Caso 2b: Si le queda quantum
      if ((*cpu_process)->burst_time - (*cpu_process)->current_burst > 0) {
        // La ráfaga en la que está no ha terminado
        (*cpu_process)->estado = RUNNING;
      } else {
        // La ráfaga ha terminado
        (*cpu_process)->num_current_complete_burst++;
        if ((*cpu_process)->num_bursts_solicitados_por_proceso -
                (*cpu_process)->num_current_complete_burst >
            0) {
          // Aún quedan ráfagas por ejecutar
          (*cpu_process)->estado = WAITING;
          (*cpu_process)->quantum =
              q; ////////////////////////////////////////////////////////////////////////////////////////////////////77
          (*cpu_process)->current_io_wait_time = (*cpu_process)->io_wait_time;
          (*cpu_process)->t_LCPU = tick;
          queue_insert_max((*cpu_process)->ultima_cola_visitada == HIGH_QUEUE
                               ? high_queue
                               : low_queue,
                           *cpu_process, (*cpu_process)->priority);
        } else {
          // El proceso termina su ejecución
          (*cpu_process)->estado = FINISHED;
          *procesos_restantes -= 1;
          (*cpu_process)->t_LCPU = tick;
          add_finished_process(finished_list, *cpu_process);
        }
        *cpu_process = NULL;
      }
    }
  }

  // 3) Ingresar los procesos a las colas según corresponda, cola low fue
  // tratada anteriormente
  for (int i = 0; i < Numero_de_procesos; i++) {
    Process *process = processes[i];
    if (process->t_inicio == tick) {
      // 3.2) Proceso ingresa por primera vez al scheduler en estado READY y a
      // la cola High
      process->quantum = 2 * q;
      queue_insert_max(high_queue, process, process->priority);
      // Incrementar el waiting_time porque ingresó ready y no sera seleccionado
      // en caso de ser seleccionado se decrementa y compensa más adelante
      process->waiting_time++;
    }
  }

  // 3.3) Para cada proceso de la cola Low, revisar si se cumple la condición
  // para subir a la cola High
  move_processes_from_low_to_high(low_queue, high_queue, tick);

  // 4) Si no hay un proceso en estado `RUNNING`, seleccionar el próximo proceso
  // de mayor prioridad
  if (*cpu_process == NULL) {
    Process *next_process = priority_process(high_queue, low_queue);
    if (next_process != NULL) {
      next_process->estado = RUNNING;
      if (next_process->num_current_complete_burst == 0) {
        next_process->response_time = tick - next_process->t_inicio;
      }
      // compensar restando una unidad de waiting_time ya que fue seleccionado
      // para la CPU
      next_process->waiting_time--;
      *cpu_process = next_process;
    }
  }
}

int main(int argc, char const *argv[]) {
  /*Lectura del input*/
  char *file_name = (char *)argv[1];
  InputFile *input_file = read_file(file_name);

  imprime_inicio(file_name, input_file);

  int tick = 0;
  int Numero_de_procesos = input_file->len;

  int procesos_restantes = Numero_de_procesos;

  Process **processes = NULL; // Lista de procesos

  Process *cpu_process = NULL;

  // ## 1) Se lee la primera línea de input:

  // - Se carga el quantum *q* asociado al scheduler, el cual se
  // utilizará en ambas colas. Por enunciado, la cola High tendrá el doble de
  // quantum que la cola Low: Quantum High = 2q, Quantum Low = q.

  // - Se carga el tiempo de inicio *T inicio* asociado al scheduler, el cual
  // se utilizará para determinar si un proceso debe ingresar a la cola High.

  int quantum = atoi(input_file->lines[0][0]);
  const int q = quantum; // Define q as the quantum value

  // se crean las colas para pasarlas al scheduler, cada cola recibe el
  // quantum, high recibe 2*quantum y low recibe quantum

  Queue *high_queue = create_queue(Numero_de_procesos);
  Queue *low_queue = create_queue(Numero_de_procesos);
  FinishedProcessList *finished_list = create_finished_processes_list();
  Process *cpu_process = NULL;

  // 	## 2) Se leen las líneas de input posteriores a la primera, donde
  // encontraremos la información asociada a los procesos.

  // - Se crean los procesos correspondientes al `struct process`, aun no se
  // ingresan al Scheduler.
  // - Hay que tener en consideración que cada proceso tiene un tiempo
  // `T_INICIO`, el cual es el tiempo en el que el proceso entra a la cola por
  // primera vez.

  for (int i = 0; i < Numero_de_procesos; ++i) {
    Process *process = create_process(input_file->lines[i]);
    if (processes == NULL) {
      processes = (Process **)calloc(1, sizeof(Process *));
    } else {
      processes = (Process **)realloc(processes, (i + 1) * sizeof(Process *));
    }
    processes[i] = process;
  };

  // 	## 3) Se crea el Scheduler, el cual recibe las colas High y Low, el

  while (1) {

    // Manejar la llegada de nuevos procesos
    for (int i = 0; i < Numero_de_procesos; i++) {
      Process *process = processes[i];
      if (process->t_inicio == tick) {
        // Cambiar el estado a READY
        process->estado = READY;

        // Calcular la prioridad
        int priority = (tick - process->t_LCPU) - process->deadline;

        // Insertar en la cola de alta prioridad
        queue_insert_max(high_queue, process, priority); /////////////
      }
    }
    // Ejecutar el scheduler
    scheduler(high_queue, low_queue, &cpu_process, tick, finished_list,
              &procesos_restantes, &Numero_de_procesos, processes);

    // Verificar si todos los procesos han terminado
    if (Numero_de_procesos == 0) {
      break;
    }

    // Incrementar el tick
    tick++;
  }

  // 5. Procesar la lista de procesos finalizados y generar el informe
  walk_in_finished_processes(finished_list);

  // 6. Liberar recursos y memoria asignada
  free_finished_processes_list(finished_list);
  for (int i = 0; i < Numero_de_procesos; i++) {
    free(processes[i]);
  }
  free(processes);
  queue_free(high_queue);
  queue_free(low_queue);
  input_file_destroy(input_file);

  return 0;
}
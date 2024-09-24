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

// 1 crear funcion que revisa si  hay Actualizar los procesos que hayan
// terminado su tiempo de espera de I/O de WAITING a READY.

void update_waiting_processes(Queue *queue, int Tactual) {
  for (int i = 0; i < queue->capacity; i++) {
    Node *node = queue->nodes[i];
    Process *actual_pivot = get_process_by_id(node->id);
    if (actual_pivot->estado == WAITING &&
        !actual_pivot->primera_rafaga_de_cpu) {
      actual_pivot->io_wait_restante--;
      if (actual_pivot->io_wait_restante == 0) {
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

// 2 crear funcion que revisa Si hay un proceso en estado RUNNING, actualizar su
// estado segun corresponda. Esto podrıa incluir sacarlo de la CPU si su quantum
// ha expirado o ha terminado su rafaga de ejecucion.

// 3 crear funcion que ingresa los procesos a las colas del scheduler

//  3.1) Si un proceso salió de la CPU, ingresarlo a la cola que corresponda.
//  3.2) Para cada proceso p, comprobar si tick = T inicio e ingresarlo a la
//  cola High. 3.3) Para cada proceso de la cola Low, revisar si se cumple la
//  condición para subir a la cola High y cambiarlos de cola según corresponda.

// Si no hay un proceso en estado RUNNING, ingresar el proceso de mayor
// prioridad en estado READY a la CPU, esto implica ignorar a todos los que se
// encuentren en estado WAITING, sin moverlos de su posición actual.

//
void move_processes_from_low_to_high(Queue *low_queue, Queue *high_queue,
                                     int Tactual) {
  // Verificar si hay que mover procesos de la cola baja a la alta
  for (int i = 0; i < low_queue->capacity; i++) {
    Process *process = low_queue->nodes[i]->process;
    if (process->estado != READY) {
      if (2 * process->t_deadline < (Tactual - process->t_LCPU)) {
        // Mover proceso a la cola de alta prioridad
        int priority = low_queue->nodes[i]->priority;
        // Remover de la cola baja
        Node *node = low_queue->nodes[i];
        low_queue->nodes[i] = low_queue->nodes[low_queue->capacity - 1];
        low_queue->capacity--;
        build_max_heap(low_queue);
        // Insertar en la cola alta
        queue_insert_max(high_queue, process, priority);
        free(node);
        i--; // Ajustar el índice
      }
    }
  }
}

// Función para actualizar el estado de los procesos que han terminado su tiempo
// de espera de I/O
void update_priorities(Queue *high_queue, Queue *low_queue, int Tactual) {
  // Actualizar prioridades en la cola de alta prioridad
  for (int i = 0; i < high_queue->capacity; i++) {
    Process *process = high_queue->nodes[i]->process;
    if (process->estado == READY) {
      int priority = (Tactual - process->t_LCPU) - process->deadline;
      high_queue->nodes[i]->priority = priority;
    }
  }
  build_max_heap(high_queue);

  // Actualizar prioridades en la cola de baja prioridad
  for (int i = 0; i < low_queue->capacity; i++) {
    Process *process = low_queue->nodes[i]->process;
    if (process->estado == READY) {
      int priority = (Tactual - process->t_LCPU) - process->deadline;
      low_queue->nodes[i]->priority = priority;
    }
  }
  build_max_heap(low_queue);
}

///
Process *select_next_process(Queue *high_queue, Queue *low_queue) {
  // Intentar extraer de la cola de alta prioridad
  Node *node = queue_extract_max(high_queue);
  if (node != NULL) {
    Process *process = get_process_by_id(node->id);
    free(node);
    return process;
  }

  // Si no hay procesos en la cola alta, intentar con la cola baja
  node = queue_extract_max(low_queue);
  if (node != NULL) {
    Process *process = get_process_by_id(node->id);
    free(node);
    return process;
  }

  // No hay procesos listos
  return NULL;
}

void scheduler(Queue *high_queue, Queue *low_queue, Process **cpu_process,
               int Tactual, FinishedProcessList *finished_list) {
  // Actualizar procesos en WAITING
  update_waiting_processes(high_queue, Tactual);
  update_waiting_processes(low_queue, Tactual);

  // Actualizar prioridades de los procesos
  update_priorities(high_queue, low_queue, Tactual);

  // Mover procesos de la cola baja a la alta si corresponde
  move_processes_from_low_to_high(low_queue, high_queue, Tactual);

  // Actualizar proceso en RUNNING
  if (*cpu_process != NULL) {
    (*cpu_process)->quantum_restante--;
    (*cpu_process)->burst_restante--;

    if ((*cpu_process)->burst_restante == 0) {
      (*cpu_process)->num_bursts--;
      if ((*cpu_process)->num_bursts == 0) {
        // procesos restantes -1

        // si no hay mas procesos restantes se termina la ejecucion
        (*cpu_process)->estado = FINISHED;
        (*cpu_process)->t_LCPU = Tactual + 1;
        // Agregar a la lista de procesos finalizados

        // #####################
        // #####################

        // calcular deadline
        // #####################
        // #####################
        add_finished_process(finished_list, *cpu_process);
        *cpu_process = NULL;
      } else {
        (*cpu_process)->estado = WAITING;
        (*cpu_process)->io_wait_restante = (*cpu_process)->io_wait_time;
        (*cpu_process)->= false;
        (*cpu_process)->t_LCPU = Tactual + 1;
        *cpu_process = NULL;
      }
    } else if ((*cpu_process)->quantum_restante == 0) {
      (*cpu_process)->estado = READY;
      (*cpu_process)->t_LCPU = Tactual + 1;
      int priority =
          (Tactual + 1 - (*cpu_process)->t_LCPU) - (*cpu_process)->deadline;
      // Mover a la cola baja
      queue_insert_max(low_queue, (*cpu_process)->pid, priority);
      *cpu_process = NULL;
    }
  }

  // #####################################################################################################

  // Seleccionar próximo proceso si la CPU está libre
  if (*cpu_process == NULL) {
    Process *next_process = select_next_process(high_queue, low_queue);
    if (next_process != NULL) {
      next_process->estado = RUNNING;
      if (next_process->primera_rafaga_de_cpu ||
          2 * next_process->deadline >= (Tactual - next_process->t_LCPU)) {
        next_process->quantum_restante = HIGH_QUEUE_QUANTUM;
      } else {
        next_process->quantum_restante = LOW_QUEUE_QUANTUM;
      }
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

  Process **processes = NULL;
  Process *cpu_process = NULL;

  // ## 1) Se lee la primera línea de input:

  // - Se carga el quantum_restante *q* asociado al scheduler, el cual se
  // utilizará en ambas colas. Por enunciado, la cola High tendrá el doble de
  // quantum que la cola Low: Quantum High = 2q, Quantum Low = q.

  // - Se carga el tiempo de inicio *T inicio* asociado al scheduler, el cual
  // se utilizará para determinar si un proceso debe ingresar a la cola High.

  int quantum = atoi(input_file->lines[0][0]);

  // se crean las colas para pasarlas al scheduler, cada cola recibe el
  // quantum, high recibe 2*quantum y low recibe quantum

  Queue *high_queue = queue_create(10);
  Queue *low_queue = queue_create(10);
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
        int priority = (tick - process->t_LCPU) - process->t_deadline;

        // Insertar en la cola de alta prioridad
        queue_insert_max(high_queue, process, priority); /////////////
      }
    }
    // Ejecutar el scheduler
    scheduler(high_queue, low_queue, &cpu_process, tick, finished_list);

    // Verificar si todos los procesos han terminado
    bool all_finished = true;
    for (int i = 0; i < Numero_de_procesos; i++) {
      if (processes[i]->estado != FINISHED) {
        all_finished = false;
        break;
      }
    }
    if (all_finished && cpu_process == NULL) {
      break;
    }

    // Incrementar el tick
    tick++;
  }

  // Al final de la simulación, procesar los procesos finalizados
  walk_in_finished_processes(finished_list);

  // Liberar recursos
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
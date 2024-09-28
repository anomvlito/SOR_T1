#include "../file_manager/manager.h"
#include "../finished_processes/finished_processes.h"
#include "../process/process.h"
// #include "../queue/queue.h"

#include <stdbool.h> // bool, true, false
#include <stddef.h>
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> // strtok, strcpy, etc.

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

void move_processes_from_low_to_high(Queue *low_queue, Queue *high_queue,
                                     int tick) {
  Node *node = low_queue->first_process;
  Node *next_node;

  while (node != NULL) {
    next_node = node->next;
    int deadline = node->process->deadline;
    int t_lcpu = node->process->t_LCPU;
    if (2 * deadline <= tick - t_lcpu) {
      Process *process = extract_process(low_queue, node->process);
      process->ultima_cola_visitada = HIGH_QUEUE; // Update here
      add_node(high_queue, create_node(process));
    }
    node = next_node;
  }
}

// Función para actualizar el estado de los procesos que han terminado su tiempo
// de espera de I/O
void update_waiting_processes(Process **processes, int total_processes,
                              Queue *high_queue, Queue *low_queue, int tick,
                              int q) {
  for (int i = 0; i < total_processes; i++) {
    Process *process = processes[i];
    if (process->estado == WAITING) {
      if (process->current_io_wait_time > 0) {
        process->current_io_wait_time--;
      }
      if (process->current_io_wait_time == 0) {
        process->estado = READY;
        process->t_LCPU =
            tick; // Actualizar t_LCPU al momento de volver a READY
        // Agregar a la cola correspondiente
        if (process->ultima_cola_visitada == HIGH_QUEUE) {
          add_node(high_queue, create_node(process));
          process->quantum = 2 * q; // Reiniciar quantum para la cola High
        } else {
          add_node(low_queue, create_node(process));
          process->quantum = q; // Reiniciar quantum para la cola Low
        }
      }
    }
  }
}

/// ocupa la funcion extraer_prioritario
Process *priority_process(Queue *high_queue, Queue *low_queue, int tick) {
  Process *process = extraer_prioritario(high_queue, tick);
  if (process != NULL) {
    // Sacar el proceso de la cola high
    extract_process(high_queue, process);
    process->ultima_cola_visitada = HIGH_QUEUE; // Update here
  } else {
    process = extraer_prioritario(low_queue, tick);
    if (process != NULL) {
      // Sacar el proceso de la cola low
      extract_process(low_queue, process);
      process->ultima_cola_visitada = LOW_QUEUE; // Update here
    }
  }
  return process;
}

void scheduler(Queue *high_queue, Queue *low_queue, Process **cpu_process,
               int tick, Queue *finished_queue, int *procesos_restantes, int q,
               Process **processes, int total_processes) {

  update_waiting_processes(processes, total_processes, high_queue, low_queue,
                           tick, q);

  for (int i = 0; i < total_processes; i++) {
    Process *process = processes[i];
    if (process->estado == READY || process->estado == WAITING) {
      process->waiting_time++;
    }
  }

  if (*cpu_process != NULL) {
    (*cpu_process)->quantum--;
    if ((*cpu_process)->quantum < 0) {
      (*cpu_process)->quantum = 0;
    }
    (*cpu_process)->current_burst++;

    if ((*cpu_process)->estado == RUNNING &&
        tick > (*cpu_process)->t_inicio + (*cpu_process)->deadline) {
      (*cpu_process)->deadline_sum =
          tick - ((*cpu_process)->t_inicio + (*cpu_process)->deadline);
    }

    // Caso 2a: Si no queda quantum
    if ((*cpu_process)->quantum == 0) {
      if ((*cpu_process)->quantum == 0 &&
          (*cpu_process)->current_burst < (*cpu_process)->burst_time) {
        // La ráfaga no ha terminado
        (*cpu_process)->interrupciones++; // Aumentar interrupciones
        (*cpu_process)->estado = READY;   // Proceso sigue en estado READY
        (*cpu_process)->quantum = q; // Reiniciar el quantum para la cola low
        (*cpu_process)->t_LCPU = tick;

        // Inserta el proceso en la cola low
        (*cpu_process)->ultima_cola_visitada = LOW_QUEUE;
        add_node(low_queue, create_node(*cpu_process));
        *cpu_process = NULL; // Liberar la CPU
      } else {
        // La ráfaga ha terminado
        (*cpu_process)->num_current_complete_burst++;
        (*cpu_process)->current_burst = 0;
        if ((*cpu_process)->num_current_complete_burst <
            (*cpu_process)->num_bursts_solicitados_por_proceso) {
          // Aún quedan ráfagas por ejecutar
          (*cpu_process)->estado = WAITING;
          (*cpu_process)->current_io_wait_time = (*cpu_process)->io_wait_time;
          (*cpu_process)->quantum =
              ((*cpu_process)->ultima_cola_visitada == HIGH_QUEUE) ? (2 * q)
                                                                   : q;

          (*cpu_process)->current_io_wait_time = (*cpu_process)->io_wait_time;
          (*cpu_process)->t_LCPU = tick;

          // Inserta el proceso en la cola correspondiente
          add_node(((*cpu_process)->ultima_cola_visitada == HIGH_QUEUE)
                       ? high_queue
                       : low_queue,
                   create_node(*cpu_process));
          *cpu_process = NULL; // Liberar la CPU
        } else {
          // El proceso termina su ejecución
          (*cpu_process)->estado = FINISHED;
          (*cpu_process)->turnaround_time = tick + 1 - (*cpu_process)->t_inicio;
          (*procesos_restantes) -= 1;
          (*cpu_process)->t_LCPU = tick;

          add_node(finished_queue, create_node(*cpu_process));
          *cpu_process = NULL; // Liberar la CPU
        }
      }
    } else {
      // Caso 2b: Si le queda quantum
      if ((*cpu_process)->burst_time - (*cpu_process)->current_burst > 0) {
        // La ráfaga en la que está no ha terminado
        (*cpu_process)->estado = RUNNING; // Mantenerlo en la CPU
      } else {
        // La ráfaga ha terminado
        (*cpu_process)->num_current_complete_burst++;
        (*cpu_process)->current_burst = 0;
        if ((*cpu_process)->num_current_complete_burst <
            (*cpu_process)->num_bursts_solicitados_por_proceso) {
          // Aún quedan ráfagas por ejecutar
          (*cpu_process)->estado = WAITING;
          (*cpu_process)->quantum =
              ((*cpu_process)->ultima_cola_visitada == HIGH_QUEUE) ? (2 * q)
                                                                   : q;
          (*cpu_process)->current_io_wait_time = (*cpu_process)->io_wait_time;
          (*cpu_process)->t_LCPU = tick;

          // Inserta el proceso en la cola correspondiente
          add_node(((*cpu_process)->ultima_cola_visitada == HIGH_QUEUE)
                       ? high_queue
                       : low_queue,
                   create_node(*cpu_process));
          *cpu_process = NULL; // Liberar la CPU
        } else {
          // El proceso termina su ejecución
          (*cpu_process)->estado = FINISHED;
          (*procesos_restantes) -= 1;
          (*cpu_process)->t_LCPU = tick;
          add_node(finished_queue, create_node(*cpu_process));
          *cpu_process = NULL; // Liberar la CPU
        }
      }
    }
  }

  for (int i = 0; i < total_processes; i++) {
    Process *process = processes[i];
    if (process->t_inicio == tick) {
      // 3.2) Proceso ingresa por primera vez al scheduler en estado READY y a
      // la cola High
      process->estado = READY;
      process->quantum = 2 * q;

      process->t_LCPU = tick; // Actualizar el tiempo de inicio del proceso
      process->ultima_cola_visitada = HIGH_QUEUE;
      Node *new_node = create_node(process);
      add_node(high_queue, new_node);
      // Incrementar el waiting_time porque ingresó en estado READY y aún no ha
      // sido seleccionado
      // process->waiting_time++;
    }
  }

  // 3.3) Para cada proceso de la cola Low, revisar si se cumple la condición
  // para subir a la cola High
  move_processes_from_low_to_high(low_queue, high_queue, tick);

  // 4) Si no hay un proceso en estado `RUNNING`, seleccionar el próximo proceso
  // de mayor prioridad
  if (*cpu_process == NULL) {
    Process *next_process = priority_process(high_queue, low_queue, tick);
    if (next_process != NULL) {
      next_process->estado = RUNNING;
      if (next_process->num_current_complete_burst == 0) {
        if (next_process->response_time == -1) {
          next_process->response_time = tick - next_process->t_inicio;
        }
      }
      // compensar restando una unidad de waiting_time ya que fue seleccionado
      // para la CPU
      // if (next_process->waiting_time > 0) { // OJO
      //   next_process->waiting_time--;
      // }
      *cpu_process = next_process;

      // pa debuguear
      printf("El siguiente proceso %s está en estado %d en el tick %d. Tiempo "
             "de inicio: %d, Turnaround: %d, Tiempo de respuesta: %d, Tiempo "
             "de espera: %d, Quantum: %d, Tiempo de ráfaga: %d, Ráfaga actual: "
             "%d, Número de ráfagas completadas: %d,\n",
             next_process->nombre, next_process->estado, tick,
             next_process->t_inicio, next_process->turnaround_time,
             next_process->response_time, next_process->waiting_time,
             next_process->quantum, next_process->burst_time,
             next_process->current_burst,
             next_process->num_current_complete_burst);
    }
  } else {
    // 5) Si hay un proceso en estado `RUNNING`, continuar ejecutándolo
    (*cpu_process)->estado = RUNNING;
    printf(
        "Proceso %s en estado %d  seguira en el tick %d +1. Tiempo de inicio: "
        "%d, Turnaround: %d, Tiempo de respuesta: %d, Tiempo de espera: %d, "
        "Quantum: %d, Tiempo de ráfaga: %d, Ráfaga actual: %d, Número de "
        "ráfagas completadas: %d,\n",
        (*cpu_process)->nombre, (*cpu_process)->estado, tick,
        (*cpu_process)->t_inicio, (*cpu_process)->turnaround_time,
        (*cpu_process)->response_time, (*cpu_process)->waiting_time,
        (*cpu_process)->quantum, (*cpu_process)->burst_time,
        (*cpu_process)->current_burst,
        (*cpu_process)->num_current_complete_burst);
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

  Process **processes = (Process **)calloc(
      Numero_de_procesos, sizeof(Process *)); // Crear array de procesos
  Process *cpu_process = NULL;

  // ## 1) Se lee la primera línea de input:

  // - Se carga el quantum *q* asociado al scheduler, el cual se
  // utilizará en ambas colas. Por enunciado, la cola High tendrá el doble de
  // quantum que la cola Low: Quantum High = 2q, Quantum Low = q.

  // - Se carga el tiempo de inicio *T inicio* asociado al scheduler, el cual
  // se utilizará para determinar si un proceso debe ingresar a la cola High.

  int q = atoi(input_file->lines[0][0]); // Define quantum como q

  // se crean las colas para pasarlas al scheduler, cada cola recibe el
  // quantum, high recibe 2*quantum y low recibe quantum

  Queue *high_queue = create_queue(Numero_de_procesos, 2 * q);
  Queue *low_queue = create_queue(Numero_de_procesos, q);
  Queue *finished_queue = create_queue(Numero_de_procesos, 0);

  // 	## 2) Se leen las líneas de input posteriores a la primera, donde
  // encontraremos la información asociada a los procesos.

  // - Se crean los procesos correspondientes al `struct process`, aun no se
  // ingresan al Scheduler.
  // - Hay que tener en consideración que cada proceso tiene un tiempo
  // `T_INICIO`, el cual es el tiempo en el que el proceso entra a la cola por
  // primera vez.

  // se crean los procesos y se guardan en un array de procesos
  for (int i = 0; i < Numero_de_procesos; ++i) {
    processes[i] = create_process(input_file->lines[i]);
    if (processes[i] == NULL) {
      fprintf(stderr, "Error al crear el proceso %d\n", i);
      return 1;
    }
  }

  // 	##} Se crea el Scheduler, el cual recibe las colas High y Low, el

  while (procesos_restantes > 0) {
    fprintf(stderr, "empieza Tick: %d\n", tick);

    // Manejar la llegada de nuevos procesos

    // Ejecutar el scheduler
    scheduler(high_queue, low_queue, &cpu_process, tick, finished_queue,
              &procesos_restantes, q, processes, Numero_de_procesos);

    // Incrementar el tick
    tick++;
    fprintf(stderr, "termina Tick: %d\n", tick);
  }

  //  Procesar la lista de procesos finalizados y generar el informe
  FILE *outputfile = fopen("output.csv", "w");
  if (outputfile != NULL) {
    // Write CSV header
    print_finished_processes(finished_queue, outputfile);
    fclose(outputfile);
  } else {
    printf("Error al abrir el archivo de salida.\n");
  }

  //  Liberar memoriaaaaaaaaaaaaaaaaaaaaaaa
  for (int i = 0; i < Numero_de_procesos; i++) {
    free(processes[i]);
  }
  free(processes);
  queue_free(high_queue);
  queue_free(low_queue);
  queue_free(finished_queue);
  input_file_destroy(input_file);

  return 0;
}
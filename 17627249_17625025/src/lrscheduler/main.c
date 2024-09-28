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
void update_waiting_processes(Queue *queue, int tick) {
  Node *node = queue->first_process;
  while (node != NULL) {
    if (node->process->estado == WAITING) {
      if (node->process->current_io_wait_time > 0) {
        node->process->current_io_wait_time--;
      } else if (node->process->current_io_wait_time == 0) {
        node->process->estado = READY;
      }
    }
    node = node->next;
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

  // 1) Actualizar los procesos que hayan terminado su tiempo de espera de I/O
  // de `WAITING` a `READY`
  update_waiting_processes(high_queue, tick);
  update_waiting_processes(low_queue, tick);

  // Incrementar el waiting time de los procesos en estado `READY`
  Node *current_hugh = high_queue->first_process;
  while (current_hugh != NULL) {
    Process *process = current_hugh->process;
    if (process->estado == READY) {
      process->waiting_time++; // Incrementar el waiting time en una unidad
    }
    current_hugh = current_hugh->next;
  }

  Node *current_low = low_queue->first_process;
  while (current_low != NULL) {
    Process *process = current_low->process;
    if (process->estado == READY) {
      process->waiting_time++; // Incrementar el waiting time e un aunidad
    }
    current_low = current_low->next;
  }

  // 2) Si hay un proceso en estado `RUNNING`, actualizar su estado según
  // corresponda
  // Cambia `queue_insert_max` por `add_node`
  if (*cpu_process != NULL) {
    (*cpu_process)->quantum--;
    (*cpu_process)->current_burst++;

    // Caso 2a: Si no queda quantum
    if ((*cpu_process)->quantum == 0) {
      if ((*cpu_process)->burst_time - (*cpu_process)->current_burst > 0) {
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
        if ((*cpu_process)->num_bursts_solicitados_por_proceso -
                (*cpu_process)->num_current_complete_burst >
            0) {
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
    } else {
      // Caso 2b: Si le queda quantum
      if ((*cpu_process)->burst_time - (*cpu_process)->current_burst > 0) {
        // La ráfaga en la que está no ha terminado
        (*cpu_process)->estado = RUNNING; // Mantenerlo en la CPU
      } else {
        // La ráfaga ha terminado
        (*cpu_process)->num_current_complete_burst++;
        if ((*cpu_process)->num_bursts_solicitados_por_proceso -
                (*cpu_process)->num_current_complete_burst >
            0) {
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
          procesos_restantes -= 1; // Cambio realizado aquí
          (*cpu_process)->t_LCPU = tick;
          add_node(finished_queue, create_node(*cpu_process));
          *cpu_process = NULL; // Liberar la CPU
        }
      }
    }
  }

  // 3) Ingresar los procesos a las colas según corresponda, cola low fue
  // tratada anteriormente
  for (int i = 0; i < total_processes; i++) {
    Process *process = processes[i];
    if (process->t_inicio == tick) {
      // 3.2) Proceso ingresa por primera vez al scheduler en estado READY y a
      // la cola High
      process->estado = READY;
      process->quantum = 2 * q;

      process->t_LCPU = tick; // Actualizar el tiempo de inicio del proceso
      Node *new_node = create_node(process);
      add_node(high_queue, new_node);
      // Incrementar el waiting_time porque ingresó en estado READY y aún no ha
      // sido seleccionado
      process->waiting_time++;
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

  // 	## 3) Se crea el Scheduler, el cual recibe las colas High y Low, el

  while (procesos_restantes > 0) {

    // Manejar la llegada de nuevos procesos
    for (int i = 0; i < Numero_de_procesos; i++) {
      Process *process = processes[i];
      if (process->t_inicio ==
          tick) { // Revisar si el proceso ingresa en este tick
        // Cambiar el estado a READY
        process->estado = READY;
        process->quantum = 2 * q; // Asignar el quantum para la cola high
        process->t_LCPU = tick;   // Actualizar el tiempo de inicio del proceso

        // Crear un nodo para el proceso y agregarlo a la cola high
        process->ultima_cola_visitada = HIGH_QUEUE;
        add_node(high_queue, create_node(process));

        // Incrementar el waiting_time porque ingresó en estado READY y aún no
        // ha sido seleccionado
        process->waiting_time++;
      }
    }
    // Ejecutar el scheduler
    scheduler(high_queue, low_queue, &cpu_process, tick, finished_queue,
              &procesos_restantes, q, processes, Numero_de_procesos);

    // Incrementar el tick
    tick++;
  }

  // 5. Procesar la lista de procesos finalizados y generar el informe
  FILE *outputfile = fopen("output.txt", "w");
  if (outputfile != NULL) {
    print_finished_processes(finished_queue, outputfile);
    fclose(outputfile);
  } else {
    printf("Error al abrir el archivo de salida.\n");
  }

  // 6. Liberar recursos y memoria asignada

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
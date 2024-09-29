#include "process.h"
#include <string.h>
Process *create_process(char **line) {
  Process *process = calloc(1, sizeof(Process));

  if (!process) {
    return NULL; // Verificar que la asignación de memoria haya sido exitosa
  }

  // Desglosar y asignar valores desde la línea de entrada
  strncpy(process->nombre, line[0], sizeof(process->nombre) - 1);
  process->nombre[sizeof(process->nombre) - 1] = '\0';
  process->pid = atoi(line[1]);
  process->t_inicio = atoi(line[2]);
  process->estado = READY;                    // Estado inicial
  process->ultima_cola_visitada = HIGH_QUEUE; // Cola inicial
  process->burst_time = atoi(line[3]);
  process->num_bursts_solicitados_por_proceso = atoi(line[4]);
  process->io_wait_time = atoi(line[5]);
  process->deadline = atoi(line[6]);

  // Inicializar el resto con valores predeterminados
  process->ultima_cola_visitada = HIGH_QUEUE;
  process->quantum = 0;
  process->interrupciones = 0;
  process->turnaround_time = 0;
  process->response_time = -1;
  process->waiting_time = 0;
  process->deadline_sum = 0;
  process->num_current_complete_burst = 0;
  process->current_burst = 0;
  process->current_io_wait_time = 0;
  process->t_LCPU = 0;

  return process;
}

Queue *create_queue(int capacity, int quantum) {
  Queue *queue = (Queue *)calloc(1, sizeof(Queue));
  queue->first_process = NULL;
  queue->last_process = NULL;
  queue->len = 0;
  queue->quantum = quantum;
  return queue;
}

Node *create_node(Process *process) {
  Node *new_node = (Node *)calloc(1, sizeof(Node));
  new_node->process = process;
  new_node->next = NULL;
  new_node->previous = NULL;
  return new_node;
}

void add_node(Queue *queue, Node *node) {
  if (queue->first_process == NULL) {
    queue->first_process = node;
    queue->last_process = node;
  } else {
    queue->last_process->next = node;
    node->previous = queue->last_process;
    queue->last_process = node;
  }
}

Process *extract_process(Queue *queue, Process *process) {
  Node *actual_node = queue->first_process;
  Node *previous_node = NULL;

  while (actual_node != NULL) {
    if (actual_node->process == process) {
      // lo encuentra y lo borra
      if (previous_node == NULL) {
        // es el primer elemento por lo q el segundo pasara a ser primero
        queue->first_process = actual_node->next;
        if (actual_node->next != NULL) {
          // no es el ultimo y no tendra previous ya que se elimino el primero
          actual_node->next->previous = NULL;
        } else {
          // es el unico y ultimo por lo tanto se elimina el ultimo
          queue->last_process = NULL;
        }
      } else {
        // tiene previous, no es el  primero
        //  el next del previous sera el next de el
        previous_node->next = actual_node->next;
        if (actual_node->next != NULL) {
          // no era el ultimo
          actual_node->next->previous = previous_node;
        } else {
          // era el ultimo
          queue->last_process = previous_node;
        }
      }
      Process *extracted_process = actual_node->process;
      free(actual_node);
      return extracted_process;
    }
    previous_node = actual_node;
    actual_node = actual_node->next;
  }
  return NULL;
}

// process.c
Process *extraer_prioritario(Queue *queue, int tick) {
  if (queue == NULL || queue->first_process == NULL) {
    return NULL;
  }

  Node *current = queue->first_process;
  Node *prioritario = NULL;
  int max_priority_value = -1;

  while (current != NULL) {
    Process *process = current->process;
    if (process->estado == READY) {
      int priority_value = (tick - process->t_LCPU) - process->deadline;
      if (prioritario == NULL || priority_value > max_priority_value) {
        prioritario = current;
        max_priority_value = priority_value;
      }
    }
    current = current->next;
  }

  if (prioritario != NULL) {
    return prioritario->process;
  }

  return NULL;
}

void queue_free(Queue *queue) {
  Node *current_node = queue->first_process;
  while (current_node != NULL) {
    Node *next_node = current_node->next;
    // No liberar current_node->process aquí
    free(current_node);
    current_node = next_node;
  }
  free(queue);
}

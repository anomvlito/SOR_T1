#include "finished_processes.h"

#include "../file_manager/manager.h"
#include "../process/process.h"
#include <stdio.h> // FILE, fopen, fclose, etc

FinishedProcessList *create_finished_finished_processes_list() {
  FinishedProcessList *list =
      (FinishedProcessList *)calloc(1, sizeof(FinishedProcessList));
  list->head = NULL;
  return list;
}

void add_finished_process(FinishedProcessList *list, Process *process) {
  FinishedProcessNode *new_node =
      (FinishedProcessNode *)calloc(1, sizeof(FinishedProcessNode));
  new_node->process = process;
  new_node->next = NULL;

  if (list->head == NULL) {
    // La lista está vacía, el nuevo nodo es la cabeza
    list->head = new_node;
  } else {
    // Recorrer la lista hasta el final
    FinishedProcessNode *current = list->head;
    while (current->next != NULL) {
      current = current->next;
    }
    // Agregar el nuevo nodo al final
    current->next = new_node;
  }
}

void print_finished_processes(Queue *queue, FILE *outputfile) {
  Node *node = queue->first_process;

  while (node != NULL) {
    Process *process = node->process;
    if (process != NULL) {
      char *nombre = process->nombre;
      int pid = process->pid;
      int interrupciones = process->interrupciones;
      int turnaround = process->turnaround_time;
      int response = process->response_time;
      int waiting = process->waiting_time;
      int deadline_sum = process->deadline_sum;

      fprintf(outputfile, "%s,%d,%d,%d,%d,%d,%d\n", nombre, pid, interrupciones,
              turnaround, response, waiting, deadline_sum);
      ;
    } else {
      fprintf(outputfile,
              "Proceso nulo encontrado en la cola de finalizados.\n");
    }
    node = node->next;
  }
}

void free_finished_processes_list(FinishedProcessList *list) {
  FinishedProcessNode *current = list->head;
  while (current != NULL) {
    FinishedProcessNode *temp = current;
    current = current->next;
    free(temp);
  }
  free(list);
}
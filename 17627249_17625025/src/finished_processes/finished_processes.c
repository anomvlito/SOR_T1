#include "finished_processes.h"

#include "../file_manager/manager.h"
#include "../process/process.h"
#include "../queue/queue.h"
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

void walk_in_finished_processes(FinishedProcessList *list) {
  FinishedProcessNode *current = list->head;

  while (current != NULL) {
    Process *process = current->process;
    // Procesar el proceso (imprimir información, calcular estadísticas, etc.)
    printf("Proceso %d finalizó en el tick \n", process->pid);

    // #####################################################################################################
    // #####################################################################################################
    // #####################################################################################################

    //                completar

    // #####################################################################################################
    // #####################################################################################################
    // #####################################################################################################

    // Avanzar al siguiente nodo
    current = current->next;
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
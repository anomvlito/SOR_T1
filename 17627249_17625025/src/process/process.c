#include "process.h"


Process *create_process(char name, int pid, estado_t estado, int burst_time,
                        int num_bursts, int io_wait_time, int deadline) {
  Process *process = calloc(1, sizeof(Process));
  strncpy(process->nombre, name, sizeof(process->nombre) - 1);
  process->nombre[sizeof(process->nombre) - 1] = '\0';
  process->pid = pid;
  process->estado = estado;
  process->burst_time = burst_time;
  process->num_bursts_solicitados_por_proceso = num_bursts;
  process->io_wait_time = io_wait_time;
  process->deadline = deadline;
  return process;
};



void add_node(Queue *queue, Node* node) {
    if (queue->first_process == NULL) {
        queue->first_process = node;
        queue->last_process = node;
    } else {
        queue->last_process->next = node;
        node->previous = queue->last_process;
        queue->last_process = node;
    }
}

void extract_process(Queue* queue, Process* process) {
  Node* actual_node = queue->first_process;
  Node* previous_node = NULL;

  while (actual_node != NULL) {
    if (actual_node->process == process) {
      // lo encuentra y lo borra
      if (previous_node == NULL) {
          // es el primer elemento por lo q el segundo pasara a ser primero
          queue->first_process = actual_node->next;
          if (actual_node->next != NULL) {
              // no es el ultimo y no tendra previous ya que se elimino el primero
              actual_node->next->previous = NULL;
          } 
          else {
              // es el unico y ultimo por lo tanto se elimina el ultimo
              queue->last_process = NULL;
          }
      } 
      else {
          //tiene previous, no es el  primero
          // el next del previous sera el next de el
          previous_node->next = actual_node->next;
          if (actual_node->next != NULL) {
              //no era el ultimo
              actual_node->next->previous = previous_node;
          } 
          else {
              //era el ultimo
              queue->last_process = previous_node;
          }
      }
      free(actual_node);
    }
    previous_node = actual_node;
    actual_node = actual_node->next;
  }
}


Process *extraer_prioritario(Queue *queue, int tick) {
    Node* temp_node = calloc(1, sizeof(Node));
    Node* priority_node = calloc(1, sizeof(Node));
    temp_node = queue->first_process;

    while (temp_node != NULL){
        if (temp_node->process->estado != WAITING){
            if (tick - priority_node->process->t_LCPU - priority_node->process->deadline 
            < tick - temp_node->process->t_LCPU - temp_node->process->deadline){ 
              priority_node = temp_node;}

            else if (tick - priority_node->process->t_LCPU - priority_node->process->deadline 
            == tick - temp_node->process->t_LCPU - temp_node->process->deadline){
                //escoge el que tiene menor pid
                if (priority_node->process->pid < temp_node->process->pid){ 
                priority_node = temp_node;
                }
            }

            temp_node = temp_node->next;
        }

      
    }


}

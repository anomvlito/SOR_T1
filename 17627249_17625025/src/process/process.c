#include "process.h"


Process *create_process(char *name, int pid, int t_inicio, estado_t estado,
                        int burst_time, int num_bursts, int io_wait_time,
                        int deadline) {
  Process *process = calloc(1, sizeof(Process));
  strncpy(process->nombre, name, sizeof(process->nombre) - 1);
  process->nombre[sizeof(process->nombre) - 1] = '\0';
  process->pid = pid;
  process->t_inicio = t_inicio;
  process->estado = READY;
  process->ultima_cola_visitada = HIGH_QUEUE;
  process->burst_time = burst_time;
  process->num_bursts_solicitados_por_proceso = num_bursts;
  process->io_wait_time = io_wait_time;
  process->deadline = deadline;
  process->quantum = 0;
  process->interrupciones = 0;
  process->turnaround_time = 0;
  process->response_time = 0;
  process->waiting_time = 0;
  process->deadline_sum = 0;
  process->num_current_complete_burst = 0;
  process->current_burst = 0;
  process->current_io_wait_time = 0;
  process->t_LCPU = 0;
  return process;
}


void add_process(Queue *queue, Process* process) {
    Node* node = calloc(1, sizeof(Node));
    if (queue->first_process == NULL) {
        queue->first_process = node;
        queue->last_process = node;
    } 
    else {
        queue->last_process->next = node;
        node->previous = queue->last_process;
        queue->last_process = node;
    }
}


Process* extract_process(Queue* queue, Process* process) {
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
              return actual_node->process;
          } 
          else {
              // es el unico y ultimo por lo tanto se elimina el ultimo
              queue->last_process = NULL;
              return actual_node->process;
          }
      } 
      else {
          //tiene previous, no es el  primero
          // el next del previous sera el next de el
          previous_node->next = actual_node->next;
          if (actual_node->next != NULL) {
              //no era el ultimo
              actual_node->next->previous = previous_node;
              return actual_node->process;
          } 
          else {
              //era el ultimo
              queue->last_process = previous_node;
              return actual_node->process;
          }
      }
    }
    previous_node = actual_node;
    actual_node = actual_node->next;
  }
}


Process *extraer_prioritario(Queue *queue, int tick) {
    Node* priority_node = NULL;
    Node* temp_node = queue->first_process;

    while (temp_node != NULL){
        if (temp_node->process->estado != WAITING){
            if (tick - priority_node->process->t_LCPU - priority_node->process->deadline 
            < tick - temp_node->process->t_LCPU - temp_node->process->deadline){ 
              priority_node = temp_node;
            }
            else if (tick - priority_node->process->t_LCPU - priority_node->process->deadline 
            == tick - temp_node->process->t_LCPU - temp_node->process->deadline){
                // si el deadline es igual escoge el de menor pid
                if (priority_node->process->pid > temp_node->process->pid){ 
                priority_node = temp_node;
                }
            }
        }
        temp_node = temp_node->next;
    }

    if (priority_node == NULL){
        Process* process = NULL;
        return process;
    }
    else {
        return priority_node->process;
    }
}


void freeQueue(Queue* queue) {
    Node* node = queue->first_process;

    while (node != NULL) {
        free(node->process);
        Node* next = node->next;
        free(node);
        node = next;
    }

    free(queue);
}

void print_finished_processes(Queue* queue, FILE* outputfile){
    Node* node = queue->first_process;

    while (node != NULL) {
        char nombre = node->process->nombre;
        int pid = node->process->pid;
        int interrupciones = node->process->interrupciones;
        int turnaround = node->process->turnaround_time;
        int response = node->process->response_time;
        int waiting = node->process->waiting_time;
        int deadline_sum = deadline_sum;

        fprintf(outputfile, "%s, %d, %d, %d, %d, %d, %d\n", 
        nombre, pid, interrupciones, turnaround, response, waiting, deadline_sum);
        node = node->next;
    }
}

void move_processes_from_low_to_high(Queue* low_queue, Queue* high_queue, int tick){
    Node* node = low_queue->first_process;

    while (node != NULL) {
        int deadline = node->process->deadline;
        int t_lcpu = node->process->t_LCPU;
        
        if (2 * deadline < tick - t_lcpu){
          Node* temp_node = node;
          Process* process = extract_process(low_queue, node->process);
          add_process(high_queue, node->process);
          node = temp_node;
        }
        node = node->next;
    }    
}

void update_waiting_processes(Queue *queue, int tick) {
    Node* node = queue->first_process;

    while (node != NULL) {
        if (node->process->estado == WAITING){
            if (node->process->current_io_wait_time > 0){
                node->process->current_io_wait_time --;
            }
            else if(node->process->current_io_wait_time == 0){
                node->process->estado == READY;
            }
        }
        node = node->next;
    } 
}
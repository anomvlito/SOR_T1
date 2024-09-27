#pragma once
#ifndef FINISHED_PROCESSES_H
#define FINISHED_PROCESSES_H

#include "../process/process.h"
#include <stdlib.h>

typedef struct finished_process_node {
  Process *process;
  struct finished_process_node *next;
} FinishedProcessNode;

typedef struct finished_process_list {
  FinishedProcessNode *head;
} FinishedProcessList;

FinishedProcessList *create_finished_processes_list();
void add_finished_process(FinishedProcessList *list, Process *process);
void walk_in_finished_processes(FinishedProcessList *list);
void free_finished_processes_list(FinishedProcessList *list);
void print_finished_processes(Queue *queue, FILE *outputfile);

#endif // FINISHED_PROCESSES_H
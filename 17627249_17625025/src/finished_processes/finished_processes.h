#pragma once
#include <stdlib.h>

typedef struct finished_process_node {
  Process *process;
  struct finished_process_node *next;
} FinishedProcessNode;

typedef struct finished_process_list {
  FinishedProcessNode *head;
  FinishedProcessNode *tail;
} FinishedProcessList;

FinishedProcessList *create_finished_finished_processes_list();
void add_finished_process(FinishedProcessList *list, Process *process);
void walk_in_finished_processes(FinishedProcessList *list);
void free_finished_processes_list(FinishedProcessList *list);

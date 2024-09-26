#pragma once
#include <stdlib.h>

typedef struct node_t {
  int id;
  int priority;
} Node;

typedef struct queue_t {
  Node **nodes;
  int size;
  int capacity;
} Queue;

Queue *create_queue(int capacity);

void queue_free(Queue *queue);

void queue_insert_max(Queue *queue, int id, int priority);

Node *queue_extract_max(Queue *queue);

void max_percolate_down(Queue *queue, int index);

void max_percolate_up(Queue *queue, int index);

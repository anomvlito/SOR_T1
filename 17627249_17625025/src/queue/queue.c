#include "queue.h"

Queue *queue_create(int size) {
  Queue *queue = (Queue *)calloc(1, sizeof(Queue));
  queue->size = size;
  queue->capacity = 0;
  queue->nodes = (Node **)calloc(size, sizeof(Node *));
  return queue;
}

void max_percolate_up(Queue *queue, int index) {
  int parent = (index - 1) / 2;
  if (index > 0 &&
      queue->nodes[index]->priority >= queue->nodes[parent]->priority) {
    if (queue->nodes[index]->priority > queue->nodes[parent]->priority) {
      Node *temp = queue->nodes[index];
      queue->nodes[index] = queue->nodes[parent];
      queue->nodes[parent] = temp;
      max_percolate_up(queue, parent);
    }
  }
}

void max_percolate_down(Queue *queue, int index) {
  int left = 2 * index + 1;
  int right = 2 * index + 2;
  int max = index;
  if (left < queue->capacity &&
      queue->nodes[left]->priority >= queue->nodes[max]->priority) {
    if (queue->nodes[left]->priority > queue->nodes[max]->priority) {
      max = left;
    }
  }
  if (right < queue->capacity &&
      queue->nodes[right]->priority >= queue->nodes[max]->priority) {
    if (queue->nodes[right]->priority > queue->nodes[max]->priority) {
      max = right;
    }
  }
  if (max != index) {
    Node *temp = queue->nodes[index];
    queue->nodes[index] = queue->nodes[max];
    queue->nodes[max] = temp;
    max_percolate_down(queue, max);
  }
}

void queue_insert_max(Queue *queue, int id, int priority) {
  if (queue->capacity == queue->size) {
    queue->size *= 2;
    queue->nodes = (Node **)realloc(queue->nodes, queue->size * sizeof(Node *));
  }
  Node *node = (Node *)calloc(1, sizeof(Node));
  node->id = id;
  node->priority = priority;
  queue->nodes[queue->capacity] = node;
  max_percolate_up(queue, queue->capacity);
  queue->capacity++;
}

Node *queue_extract_max(Queue *queue) {
  Node *max = queue->nodes[0];
  queue->nodes[0] = queue->nodes[queue->capacity - 1];
  queue->capacity--;
  max_percolate_down(queue, 0);
  return max;
}

void queue_free(Queue *queue) {
  for (int i = 0; i < queue->capacity; i++) {
    free(queue->nodes[i]);
  }
  free(queue->nodes);
  free(queue);
}

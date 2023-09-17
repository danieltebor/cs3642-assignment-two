#pragma once

#include "eight_puzzle.h"

// Queue for use in BFS.
typedef struct {
    Node* nodes[MAX_NODES];
    unsigned int rear_idx;
} Queue;

// Construct queue.
void init_queue(Queue* queue);

// Enque a node into the queue.
void enqueue(Queue* queue, Node* node);

// Deque a node from the queue.
Node* dequeue(Queue* queue);

// Return true if queue is empty.
bool is_empty(Queue* queue);
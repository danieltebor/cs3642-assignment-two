#pragma once

#include <stdbool.h>

#include "eight_puzzle.h"
#include "node.h"

// Queue for use in BFS.
typedef struct {
    Node* nodes[MAX_NODES];
    int rear_idx;
} Queue;

// Construct queue.
void init_queue(Queue* queue);

// Enque a node into the queue.
void enqueue(Queue* queue, const Node* node);

// Enque a node into the priority queue.
// The node is inserted in the correct position based on its depth + heuristic.
// Lower is better.
void priority_enqueue(Queue* queue, const Node* node);

// Deque a node from the queue.
Node* dequeue(Queue* queue);

// Return true if queue is empty.
bool queue_is_empty(Queue* queue);
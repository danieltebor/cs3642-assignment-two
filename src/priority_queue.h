#pragma once

#include "eight_puzzle.h"

// Priority Queue for use in A* search and UCS search.
typedef struct {
    Node* nodes[MAX_NODES];
    unsigned int rear_idx;
} PriorityQueue;

// Construct priority queue.
void init_priority_queue(PriorityQueue* queue);

// Enque a node into the priority queue.
// The node is inserted in the correct position based on its depth + heuristic.
// Lower is better.
void p_enqueue(PriorityQueue* queue, Node* node);

// Deque a node from the priority queue.
// The node with the lowest cost is returned.
Node* p_dequeue(PriorityQueue* queue);

// Return true if queue is empty.
bool p_is_empty(PriorityQueue* queue);
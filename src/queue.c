#include "queue.h"

// Construct queue.
void init_queue(Queue* queue) {
    queue->rear_idx = 0;
}

// Enque a node into the queue.
void q_enqueue(Queue* queue, Node* node) {
    queue->nodes[queue->rear_idx++] = node;
}

// Dequeue a node from the queue.
Node* q_dequeue(Queue* queue) {
    Node* front = queue->nodes[0];

    // Shift nodes to the left.
    // Keeping track of a front and rear index would be much better for this, but that's
    // writing a fast priority queue is a bit out of the scope for this assignment.
    for (unsigned int i = 0; i < queue->rear_idx; i++) {
        queue->nodes[i] = queue->nodes[i + 1];
    }
    queue->rear_idx--;

    return front;
}

// Return true if queue is emtpy.
bool q_is_empty(Queue* queue) {
    if (queue->rear_idx == 0) {
        return true;
    }
    return false;
}
#include "queue.h"

// Construct queue.
void init_queue(Queue* queue) {
    queue->rear_idx = 0;
}

// Enque a node into the queue.
void enqueue(Queue* queue, Node* node) {
    queue->nodes[queue->rear_idx++] = node;
}

// Enque a node into the priority queue.
// The node is inserted in the correct position based on its depth + heuristic.
// Lower is better.
void priority_enqueue(Queue* queue, Node* node) {
    // If the queue is empty, insert the node at the beginning.
    if (queue->rear_idx == 0) {
        queue->nodes[queue->rear_idx++] = node;
        return;
    }

    // Find the correct position to insert the new node.
    int i;
    for (i = queue->rear_idx - 1; i >= 0; i--) {
        if (node->depth + node->heuristic < queue->nodes[i]->depth + queue->nodes[i]->heuristic) {
            // Move the existing node one position to the right.
            queue->nodes[i + 1] = queue->nodes[i];
        } else {
            break;
        }
    }

    // Insert the new node at the found position.
    queue->nodes[i + 1] = node;
    queue->rear_idx++;
}

// Dequeue a node from the queue.
Node* dequeue(Queue* queue) {
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
bool queue_is_empty(Queue* queue) {
    if (queue->rear_idx == 0) {
        return true;
    }
    return false;
}
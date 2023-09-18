#include "priority_queue.h"

// Construct priority queue.
void init_priority_queue(PriorityQueue* queue) {
    queue->rear_idx = 0;
}

// Enque a node into the priority queue.
// The node is inserted in the correct position based on its depth + heuristic.
// Lower is better.
void p_enqueue(PriorityQueue* queue, Node* node) {
    queue->nodes[++queue->rear_idx] = node;

    // Sort new node into correct position.
    // Bubble sort is used. This isn't the most efficient way to do this,
    // but using something like a heap, especially in c, is out of the scope for this assignment.
    for (unsigned int i = queue->rear_idx; i > 0; i--) {
        if (queue->nodes[i]->depth + queue->nodes[i]->heuristic
            < queue->nodes[i - 1]->depth + queue->nodes[i - 1]->heuristic) {
            Node* temp = queue->nodes[i];
            queue->nodes[i] = queue->nodes[i - 1];
            queue->nodes[i - 1] = temp;
        } else {
            break;
        }
    }
}

// Deque a node from the priority queue.
// The node with the lowest cost is returned.
Node* p_dequeue(PriorityQueue* queue) {
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
bool p_is_empty(PriorityQueue* queue) {
    if (sizeof(queue->nodes) == 0) {
        return true;
    }
    return false;
}
#include "stack.h"

// Construct stack.
void init_stack(Stack* stack) {
    stack->top_idx = -1;
}

// Push node to stack.
void push(Stack* stack, Node* node) {
    // Put node at top of internal array.
    stack->nodes[++stack->top_idx] = node;
}

// Pop node from stack.
Node* pop(Stack* stack) {
    // Remove node from top of internal array and return it.
    Node* top_node = stack->nodes[stack->top_idx];
    stack->nodes[stack->top_idx--] = NULL;
    return top_node;
}

// Empty stack.
void empty(Stack* stack) {
    for (unsigned int i = 0; i < MAX_NODES; i++) {
        stack->nodes[i] = NULL;
    }
    stack->top_idx = -1;
}

// Check if stack is empty.
bool stack_is_empty(Stack* stack) {
    return stack->top_idx == -1;
}
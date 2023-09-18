#pragma once

#include "eight_puzzle.h"

// Stack to be used in DFS.
typedef struct {
    Node* nodes[MAX_NODES];
    int top_idx;
} Stack;

// Construct stack.
void init_stack(Stack* stack);

// Push node to stack.
void push(Stack* stack, Node* node);

// Pop node from stack.
Node* pop(Stack* stack);

// Empty stack.
void empty(Stack* stack);

// Check if stack is empty.
bool s_is_empty(Stack* stack);
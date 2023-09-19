#pragma once

// Struct to represent a node in the search tree.
typedef struct Node Node;

struct Node {
    const Node* parent; // Pointer to parent node
    unsigned int state[3][3]; // Board state
    unsigned int depth; // Depth of node in search tree
    unsigned int heuristic; // Heuristic value of node
    Node* next; // Pointer to next node in linked list. Used for hashing.
};
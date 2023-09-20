#pragma once

#include <stdbool.h>

// Struct to represent a node in the search tree.
typedef struct Node Node;

struct Node {
    const Node* parent; // Pointer to parent node
    Node* next; // Pointer to next node in linked list. Used for hashing.
    unsigned int state[3][3]; // Board state
    unsigned int depth; // Depth of node in search tree
    unsigned int heuristic; // Heuristic value of node
};

// Checks if two nodes are equivalent.
bool check_states_are_equivalent(const unsigned int state1[3][3], const unsigned int state2[3][3]);
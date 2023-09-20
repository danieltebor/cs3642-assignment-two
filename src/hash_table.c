#include <string.h>
#include <stdio.h>

#include "hash_table.h"

// Hashing function to determine index in internal array.
unsigned int hash_function(const Node* node, unsigned int table_size) {
    unsigned int hash = 0;

    // Traverse state to generate unique hash.
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            // 7 is a prime number commonly used in hash functions 
            // to get a beneficial distribution in the table. (I looked it up).
            hash = 7 * hash + node->state[y][x];
        }
    }
    
    // Modulo the hash by the table_size if it larger than the table_size.
    if (hash >= table_size) {
        hash %= table_size;
    }

    return hash;
}

// Insert a node into the hash table.
void insert(Node* node, Node** nodes, unsigned int table_size) {
    // Run hashing function to get index for the node.
    unsigned int idx = hash_function(node, table_size);

    // Assign the value in nodes[idx] to hashed_node->next.
    // If that value is NULL, then hashed_node->next will be NULL.
    // This is to deal with collisions.
    node->next = nodes[idx];
    nodes[idx] = node;
}

// Find a node in the hash table.
Node* search(const Node* node, const Node** nodes, unsigned int table_size) {
    // Run hashing function to get index for the node.
    unsigned int idx = hash_function(node, table_size);

    // Traverse linked list at nodes[idx] to find the node.
    const Node* current_node = nodes[idx];
    while (current_node != NULL) {
        // Check if node is equivalent to current_node.
        if (check_states_are_equivalent(node->state, current_node->state)) {
            return (Node*) current_node;
        }
        current_node = current_node->next;
    }

    // Return NULL if node not found.
    return NULL;
}
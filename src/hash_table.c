#include "hash_table.h"

// Hashing function to determine index in internal array.
unsigned int hash_function(Node* node, unsigned int table_size) {
    unsigned int hash = 0;

    // Traverse state to generate unique hash.
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            // 37 is a prime number commonly used in hash functions 
            // to get a beneficial distribution in the table. (I looked it up).
            hash = 37 * hash + node->state[y][x];
        }
    }

    // Modulo the hash by the table_size to ensure it is within bounds.
    return hash % table_size;
}

// Insert a node into the hash table.
void insert(Node* node, Node** nodes) {
    // Run hashing function to get index for the node.
    unsigned int idx = hash_function(node, sizeof(nodes) / sizeof(nodes[0]));

    // Assign the value in nodes[idx] to hashed_node->next.
    // If that value is NULL, then hashed_node->next will be NULL.
    // This is to deal with collisions.
    node->next = nodes[idx];
    nodes[idx] = node;
}

// Find a node in the hash table.
Node* search(Node* node, Node** nodes) {
    // Run hashing function to get index for the node.
    unsigned int idx = hash_function(node, sizeof(nodes) / sizeof(nodes[0]));

    // Traverse linked list at nodes[idx] to find the node.
    Node* current_node = nodes[idx];
    while (current_node != NULL) {
        // Check if node is equivalent to current_node.
        bool nodes_are_equivalent = true;

        for (unsigned int y = 0; y < 3; y++) {
            for (unsigned int x = 0; x < 3; x++) {
                if (node->state[y][x] != current_node->state[y][x]) {
                    nodes_are_equivalent = false;
                    break;
                }
            }
            if (!nodes_are_equivalent) {
                break;
            }
        }

        // Return current_node if nodes are equivalent.
        if (nodes_are_equivalent) {
            return current_node;
        }
        
        current_node = current_node->next;
    }

    // Return NULL if node not found.
    return NULL;
}
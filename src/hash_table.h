#pragma once

#include "node.h"

// Insert a node into the hash table.
void insert(Node* node, Node** nodes, unsigned int table_size);

// Find a node in the hash table.
Node* search(const Node* node, const Node** nodes, unsigned int table_size);
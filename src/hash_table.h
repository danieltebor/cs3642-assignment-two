#pragma once

#include <stdbool.h>
#include <string.h>

#include "node.h"

// Insert a node into the hash table.
void insert(Node* node, Node** nodes);

// Find a node in the hash table.
Node* search(Node* node, Node** nodes);
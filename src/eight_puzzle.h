#pragma once

#include <stdbool.h>

#include "node.h"

// The max number of nodes is 9! since the state is a 3x3 grid with 9 tiles.
#define MAX_NODES 362880

// Goal state.
extern const unsigned int goal_state[3][3];

// Generate a node with a random starting state.
Node* generate_random_start_node();

// Return all possible children of a node.
Node** extend_node(const Node* node,
                   const Node** nodes_visited,
                   bool should_use_heuristic);
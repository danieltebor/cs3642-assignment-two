#pragma once

// The max number of nodes is 9! since the state is a 3x3 grid with 9 tiles.
// Only half of these are reachable from any given starting state, 
// so the max number of nodes is 9!/2 -> 181440.
#define MAX_NODES 181440

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hash_table.h"

// Goal state.
extern const unsigned int goal_state[3][3];

// Generate a node with a random starting state.
Node* generate_random_start_node();

// Checks if two nodes are equivalent.
bool check_states_are_equivalent(const unsigned int state1[3][3], const unsigned int state2[3][3]);

// Return all possible children of a node.
Node** extend_node(Node* node,
                   Node** nodes_visited,
                   bool should_use_misplaced_heuristic,
                   bool should_use_manhatten_distance_heuristic);
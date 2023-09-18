#pragma once

// The max number of nodes is 9! since the state is a 3x3 grid with 9 tiles.
// Only half of these are reachable from any given starting state, 
// so the max number of nodes is 9!/2 -> 181440.
#define MAX_NODES 181440

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct to represent a node in the search tree.
typedef struct Node Node;

struct Node {
    Node* parent; // Pointer to parent node
    unsigned int state[3][3]; // Board state
    unsigned int depth; // Depth of node in search tree
    unsigned int heuristic; // Heuristic value of node
};

// Goal state.
extern unsigned int goal_state[3][3];

// Generate a node with a random starting state.
Node generate_random_start_node();

// Calculates a heuristic value corresponding to the number 
// of tiles misplaced compared to the goal state
unsigned int calc_num_tiles_misplaced(Node* node);

// Calculates a heuristic value by calculating the sum of
// the Manhatten distances of all tiles in the state compared to the goal.
unsigned int calc_summed_manhatten_distances(Node* node);

// Checks if two nodes are equivalent.
bool check_states_are_equivalent(unsigned int state1[3][3], unsigned int state2[3][3]);

// Return all possible children of a node.
Node** extend_node(Node* node, bool should_use_misplaced_heuristic, bool should_use_manhatten_distance_heuristic);
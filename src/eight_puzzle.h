#pragma once

#include <stdbool.h>
#include <time.h>

// Struct to represent a node in the search tree.
typedef struct {
    Node* parent; // Pointer to parent node
    unsigned int state[3][3]; // Board state
    unsigned int cost; // Estimated cost to reach goal state
} Node;

// Goal state.
unsigned int goal_state[3][3] = {
    {1, 2, 3},
    {8, 0, 4},
    {7, 6, 5}
};

// Generate a node with a random starting state.
Node generate_random_start_node();

// Checks if two nodes are equivalent.
bool check_states_are_equivalent(unsigned int* state1[3][3], unsigned int* state2[3][3]);

// Calculates a heuristic value corresponding to the number 
// of tiles misplaced compared to the goal state
unsigned int calc_num_tiles_misplaced(Node* node);

// Calculates a heuristic value by calculating the sum of
// the Manhatten distances of all tiles in the state compared to the goal.
unsigned int calc_summed_manhatten_distances(Node* node);
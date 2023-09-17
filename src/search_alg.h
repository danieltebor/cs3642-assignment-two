#pragma once

// The max number of nodes is 9! since the state is a 3x3 grid with 9 tiles.
// Only half of these are reachable from any given starting state, 
// so the max number of nodes is 9!/2 -> 181440.
#define MAX_NODES 181440

#include "eight_puzzle.h"

Node* depth_first_search(Node* start_node);
Node* breadth_first_search(Node* start_node);
Node* uniform_cost_search(Node* start_node);
Node* a_star_search(Node* start_node);
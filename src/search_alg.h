#pragma once

#include "eight_puzzle.h"
#include "priority_queue.h"
#include "stack.h"
#include "queue.h"

typedef struct {
    Node* goal_node;
    unsigned int num_nodes_visited;
} SearchResult;

SearchResult* depth_first_search(Node* start_node);
SearchResult* breadth_first_search(Node* start_node);
SearchResult* uniform_cost_search(Node* start_node);
SearchResult* a_star_search(Node* start_node);
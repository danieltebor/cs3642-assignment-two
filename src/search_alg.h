#pragma once

#include "node.h"

typedef struct {
    unsigned int trace_size;
    unsigned int num_nodes_visited;
    double cpu_time_taken_ms;
} SearchResult;

// Frees a SearchResult from memory.
void depth_first_search(Node* start_node, SearchResult* result);
void breadth_first_search(Node* start_node, SearchResult* result);
void uniform_cost_search(Node* start_node, SearchResult* result);
void a_star_search(Node* start_node, SearchResult* result);
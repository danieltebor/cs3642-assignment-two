#pragma once

#include "eight_puzzle.h"
#include "priority_queue.h"
#include "stack.h"
#include "queue.h"

typedef struct {
    Node** trace;
    unsigned int trace_size;
    unsigned int num_nodes_visited;
} SearchResult;

// Frees a SearchResult from memory.
void free_search_result(SearchResult* result);

void depth_first_search(Node* start_node, SearchResult* result);
void breadth_first_search(Node* start_node, SearchResult* result);
void uniform_cost_search(Node* start_node, SearchResult* result);
void a_star_search(Node* start_node, SearchResult* result);
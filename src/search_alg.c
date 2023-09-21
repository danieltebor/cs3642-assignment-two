#include "search_alg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hash_table.h"
#include "stack.h"
#include "queue.h"

// Frees nodes in an array from memory.
void free_nodes(Node** nodes, unsigned int num_nodes) {
    for (unsigned int i = 0; i < num_nodes; i++) {
        if (nodes[i] != NULL) {
            Node* current_node = nodes[i];
            // Loop through Linked List of Node stored in each node.
            // This is due to overlap during hashing.
            while (current_node != NULL) {
                Node* next_node = current_node->next;
                free(current_node);
                current_node = next_node;
            }
        }
    }
}

// Each search algorithm populates a SearchResult struct.
void populate_search_result(SearchResult* result, Node** nodes_visited, const Node* goal_node, unsigned int num_nodes_visited, double cpu_time_taken_ms) {
    result->num_nodes_visited = num_nodes_visited;
    result->cpu_time_taken_ms = cpu_time_taken_ms;
    result->trace_size = 0;

    // Check if goal reached.
    if (goal_node == NULL) {
        // Free nodes_visited hash table to prevent a memory leak.
        free_nodes(nodes_visited, MAX_NODES);
        free(nodes_visited);
        return;
    }

    // Trace back from goal node to start node to get the trace size.
    const Node* current_node = goal_node;
    while (current_node != NULL) {
        current_node = current_node->parent;
        ++result->trace_size;
    }

    /*
    // Dynamically allocate trace array and populate from root to goal.
    // Pointers copied into trace array are set to NULL in nodes_visited to prevent deallocation.
    result->trace = (Node**) malloc(sizeof(Node*) * result->trace_size);
    current_node = goal_node;
    unsigned int trace_idx = result->trace_size;
    while (current_node != NULL) {
        // Copy node into trace array.
        // The copy is done so no issues arise when freeing nodes_visited.
        Node* node_copy = calloc(1, sizeof(Node));
        memcpy(node_copy->state, current_node->state, sizeof(node_copy->state));
        node_copy->depth = current_node->depth;
        node_copy->heuristic = current_node->heuristic;

        result->trace[--trace_idx] = node_copy;

        current_node = current_node->parent;
    }
    */

    // Free nodes_visited hash table to prevent a memory leak.
    free_nodes(nodes_visited, MAX_NODES);
    free(nodes_visited);
}

void depth_first_search(Node* start_node, SearchResult* result) {
    Stack nodes_to_visit;
    init_stack(&nodes_to_visit);

    Node** nodes_visited = calloc(MAX_NODES, sizeof(Node*));
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;

    // Record start time for algorithm execution.
    clock_t start_time = clock();

    // Push start node.
    // Copy start node so that it is not deallocated after each try.
    push(&nodes_to_visit, start_node);

    // Visit every node in the stack. When a node is extended in the loop,
    // the children are pushed. This causes the nodes to be visited in a DFS order.
    while (!stack_is_empty(&nodes_to_visit)) {
        Node* current_node = pop(&nodes_to_visit);
        insert(current_node, nodes_visited, MAX_NODES);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and push children.
        // 3rd parameter is false as a heuristic is not used in DFS.
        Node** child_nodes = extend_node(current_node, (const Node**) nodes_visited, false);
        
        // Push children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (child_nodes[i] != NULL) {
                push(&nodes_to_visit, child_nodes[i]);
            } else {
                break;
            }
        }

        // Free child nodes array to prevent a memory leak.
        free(child_nodes);
    }

    // Record end time for algorithm execution.
    clock_t end_time = clock();
    // Calculate time taken in milliseconds.
    double cpu_time_taken_ms = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    
    // Fill out fields in the SearchResult struct.
    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited, cpu_time_taken_ms);
}

void breadth_first_search(Node* start_node, SearchResult* result) {
    Queue nodes_to_visit;
    init_queue(&nodes_to_visit);

    Node** nodes_visited = calloc(MAX_NODES, sizeof(Node*));
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;

    // Record start time for algorithm execution.
    clock_t start_time = clock();

    // Enqueue start node.
    // Copy start node so that it is not deallocated after each try.
    enqueue(&nodes_to_visit, start_node);

    // Visit every node in the queue. When a node is extended in the loop,
    // the children are enqueued. This causes the nodes to be visited in a BFS order.
    while (!queue_is_empty(&nodes_to_visit)) {
        Node* current_node = dequeue(&nodes_to_visit);
        insert(current_node, nodes_visited, MAX_NODES);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and enqueue children.
        // 3rd parameter is false as a heuristic is not used in BFS.
        Node** child_nodes = extend_node(current_node, (const Node**) nodes_visited, false);

        // Enqueue children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (child_nodes[i] != NULL) {
                enqueue(&nodes_to_visit, child_nodes[i]);
            } else {
                break;
            }
        }

        // Free child nodes array to prevent a memory leak.
        free(child_nodes);
    }

    // Record end time for algorithm execution.
    clock_t end_time = clock();
    // Calculate time taken in milliseconds.
    double cpu_time_taken_ms = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    // Fill out fields in the SearchResult struct.
    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited, cpu_time_taken_ms);
}

void uniform_cost_search(Node* start_node, SearchResult* result) {
    Queue nodes_to_visit;
    init_queue(&nodes_to_visit);

    Node** nodes_visited = calloc(MAX_NODES, sizeof(Node*));
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;
    
    // Record start time for algorithm execution.
    clock_t start_time = clock();

    // Enqueue start node.
    priority_enqueue(&nodes_to_visit, start_node);
    
    // Visit every node in the priority queue. When a node is extended in the loop,
    // the children are enqued and internally sorted so that those with the lowest
    // depth come first. This causes the nodes to be visited in a UCS order.
    while (!queue_is_empty(&nodes_to_visit)) {
        Node* current_node = dequeue(&nodes_to_visit);
        insert(current_node, nodes_visited, MAX_NODES);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and enqueue children.
        // 3rd parameter is false as a heuristic is not used in UCS.
        Node** child_nodes = extend_node(current_node, (const Node**) nodes_visited, false);

        // Enqueue children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (child_nodes[i] != NULL) {
                priority_enqueue(&nodes_to_visit, child_nodes[i]);
            } else {
                break;
            }
        }

        // Free child nodes array to prevent a memory leak.
        free(child_nodes);
    }

    // Record end time for algorithm execution.
    clock_t end_time = clock();
    // Calculate time taken in milliseconds.
    double cpu_time_taken_ms = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    // Fill out fields in the SearchResult struct.
    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited, cpu_time_taken_ms);
}

void a_star_search(Node* start_node, SearchResult* result) {
    Queue nodes_to_visit;
    init_queue(&nodes_to_visit);

    Node** nodes_visited = calloc(MAX_NODES, sizeof(Node*));
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;
    
    // Record start time for algorithm execution.
    clock_t start_time = clock();

    // Enqueue start node.
    priority_enqueue(&nodes_to_visit, start_node);
    
    // Visit every node in the priority queue. When a node is extended in the loop,
    // the children are enqued and internally sorted so that those with the lowest
    // depth + heuristic value come first. This causes the nodes to be visited in a A* order.
    while (!queue_is_empty(&nodes_to_visit)) {
        Node* current_node = dequeue(&nodes_to_visit);
        insert(current_node, nodes_visited, MAX_NODES);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and enqueue children.
        // 3rd parameter is true as a heuristic is used in A*.
        Node** child_nodes = extend_node(current_node, (const Node**) nodes_visited, true);

        // Enqueue children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (child_nodes[i] != NULL) {
                priority_enqueue(&nodes_to_visit, child_nodes[i]);
            } else {
                break;
            }
        }

        // Free child nodes array to prevent a memory leak.
        free(child_nodes);
    }

    // Record end time for algorithm execution.
    clock_t end_time = clock();
    // Calculate time taken in milliseconds.
    double cpu_time_taken_ms = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    // Fill out fields in the SearchResult struct.
    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited, cpu_time_taken_ms);
}
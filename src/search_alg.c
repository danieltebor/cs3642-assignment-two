#include "search_alg.h"

// Frees nodes in an array from memory.
void free_nodes(Node** nodes, unsigned int num_nodes) {
    for (unsigned int i = 0; i < num_nodes; i++) {
        if (nodes[i] != NULL) {
            Node* current_node = nodes[i];
            while (current_node != NULL) {
                Node* next_node = current_node->next;
                free(current_node);
                current_node = next_node;
            }
        }
    }
}

// Frees a SearchResult from memory.
void free_search_result(SearchResult* result) {
    free_nodes(result->trace, result->trace_size);
    if (result->trace != NULL) {
        free(result->trace);
    }
}

// Each search algorithm populates a SearchResult struct.
void populate_search_result(SearchResult* result, Node** nodes_visited, Node* goal_node, unsigned int num_nodes_visited) {
    printf("ran");
    result->num_nodes_visited = num_nodes_visited;
    result->trace_size = 0;

    // Check if goal reached.
    if (goal_node == NULL) {
        // Free nodes_visited array to prevent a memory leak.
        free_nodes(nodes_visited, num_nodes_visited);
        return;
    }

    // Trace back from goal node to start node to get the trace size.
    const Node* current_node = goal_node;
    while (current_node != NULL) {
        current_node = current_node->parent;
        ++result->trace_size;
    }

    // Dynamically allocate trace array and populate from root to goal.
    // Pointers copied into trace array are set to NULL in nodes_visited to prevent deallocation.
    result->trace = (Node**) malloc(sizeof(Node*) * result->trace_size);
    current_node = goal_node;
    unsigned int trace_idx = result->trace_size;
    while (current_node != NULL) {
        // Copy node into trace array.
        // The copy is done so no issues arise when freeing nodes_visited.
        Node* node_copy = malloc(sizeof(Node));
        memcpy(node_copy, current_node, sizeof(Node));
        result->trace[--trace_idx] = node_copy;
        current_node = current_node->parent;
    }

    // Free nodes_visited array to prevent a memory leak.
    free_nodes(nodes_visited, num_nodes_visited);
}

void depth_first_search(Node* start_node, SearchResult* result) {
    Stack nodes_to_visit;
    init_stack(&nodes_to_visit);

    Node** nodes_visited = malloc(MAX_NODES * 2 * sizeof(Node*));
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;

    // Push start node.
    // Copy start node so that it is not deallocated after each try.
    push(&nodes_to_visit, start_node);

    // Visit every node in the stack. When a node is extended in the loop,
    // the children are pushed. This causes the nodes to be visited in a DFS order.
    while (!stack_is_empty(&nodes_to_visit)) {
        Node* current_node = pop(&nodes_to_visit);
        insert(current_node, nodes_visited);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and push children.
        // Both booleans are false as a heuristic is not used in DFS.
        Node** child_nodes = extend_node(current_node, nodes_visited, false, false);
        
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
    
    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited);
    
    // Free nodes_visted array to prevent a memory leak.
    free(nodes_visited);
}

void breadth_first_search(Node* start_node, SearchResult* result) {
    Queue nodes_to_visit;
    init_queue(&nodes_to_visit);

    Node** nodes_visited = malloc(MAX_NODES * 2 * sizeof(Node*));
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;

    // Enqueue start node.
    // Copy start node so that it is not deallocated after each try.
    enqueue(&nodes_to_visit, start_node);

    // Visit every node in the stack. When a node is extended in the loop,
    // the children are enqueued. This causes the nodes to be visited in a BFS order.
    while (!queue_is_empty(&nodes_to_visit)) {
        Node* current_node = dequeue(&nodes_to_visit);
        insert(current_node, nodes_visited);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and enqueue children.
        // Both booleans are false as a heuristic is not used in DFS.
        Node** child_nodes = extend_node(current_node, nodes_visited, false, false);

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

    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited);
    
    // Free nodes_visted array to prevent a memory leak.
    free(nodes_visited);
}

void uniform_cost_search(Node* start_node, SearchResult* result) {
    Queue nodes_to_visit;
    init_queue(&nodes_to_visit);

    Node** nodes_visited = malloc(MAX_NODES * sizeof(Node*));
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;
    
    // Enqueue start node.
    priority_enqueue(&nodes_to_visit, start_node);
    
    // Visit every node in the priority queue. When a node is extended in the loop,
    // the children are enqued and internally sorted so that those with the lowest
    // depth come first. This causes the nodes to be visited in a UCS order.
    while (!queue_is_empty(&nodes_to_visit)) {
        Node* current_node = dequeue(&nodes_to_visit);
        insert(current_node, nodes_visited);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and enqueue children.
        // Both booleans are false as a heuristic is not used in UCS.
        Node** child_nodes = extend_node(current_node, nodes_visited, false, false);

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

    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited);
    
    // Free nodes_visted array to prevent a memory leak.
    free(nodes_visited);
}

void a_star_search(Node* start_node, SearchResult* result) {
    Queue nodes_to_visit;
    init_queue(&nodes_to_visit);

    Node** nodes_visited = malloc(MAX_NODES * sizeof(Node*));
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;
    
    // Enqueue start node.
    priority_enqueue(&nodes_to_visit, start_node);
    
    // Visit every node in the priority queue. When a node is extended in the loop,
    // the children are enqued and internally sorted so that those with the lowest
    // depth + heuristic value come first. This causes the nodes to be visited in a A* order.
    while (!queue_is_empty(&nodes_to_visit)) {
        Node* current_node = dequeue(&nodes_to_visit);
        insert(current_node, nodes_visited);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and enqueue children.
        // Both booleans are true as a heuristic is used in A*.
        Node** child_nodes = extend_node(current_node, nodes_visited, true, true);

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

    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited);
    
    // Free nodes_visted array to prevent a memory leak.
    free(nodes_visited);
}
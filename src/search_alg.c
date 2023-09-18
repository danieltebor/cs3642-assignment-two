#include "search_alg.h"

// Frees nodes in an array from memory.
void free_nodes(Node** nodes, unsigned int num_nodes) {
    for (unsigned int i = 0; i < num_nodes; i++) {
        if (nodes[i] != NULL) {
            free(nodes[i]);
        }
    }
}

// Frees a SearchResult from memory.
void free_search_result(SearchResult* result) {
    free_nodes(result->trace, sizeof(result->trace) / sizeof(result->trace[0]));
    free(result->trace);
}

// Each search algorithm populates a SearchResult struct.
void populate_search_result(SearchResult* result, Node* nodes_visited[MAX_NODES], Node* goal_node, unsigned int num_nodes_visited) {
    result->num_nodes_visited = num_nodes_visited;
    // Check if goal reached.
    if (goal_node == NULL) {
        // Free nodes_visited array to prevent a memory leak.
        free_nodes(nodes_visited, num_nodes_visited);
        return;
    }

    // Trace back from goal node to start node to get the trace size.
    unsigned int trace_size = 0;
    Node* current_node = goal_node;
    while (current_node != NULL) {
        current_node = current_node->parent;
        ++trace_size;
    }

    // Dynamically allocate trace array and populate from root to goal.
    // Pointers copied into trace array are set to NULL in nodes_visited to prevent deallocation.
    result->trace = (Node**) malloc(sizeof(Node*) * trace_size);
    current_node = goal_node;
    while (current_node != NULL) {
        for (unsigned int i = 0; i < num_nodes_visited; i++) {
            if (check_states_are_equivalent(current_node->state, nodes_visited[i]->state)) {
                result->trace[--trace_size] = nodes_visited[i];
                nodes_visited[i] = NULL;
                break;
            }
        }
        current_node = current_node->parent;
    }

    // Free nodes_visited array to prevent a memory leak.
    free_nodes(nodes_visited, num_nodes_visited);
}

void depth_first_search(Node* start_node, SearchResult* result) {
    Stack nodes_to_visit;
    init_stack(&nodes_to_visit);

    Node* nodes_visited[MAX_NODES];
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;

    // Push start node.
    push(&nodes_to_visit, start_node);

    printf("\n");
    int counter = 0;
    // Visit every node in the stack. When a node is extended in the loop,
    // the children are pushed. This causes the nodes to be visited in a DFS order.
    while (!s_is_empty(&nodes_to_visit)) {
        Node* current_node = pop(&nodes_to_visit);
        nodes_visited[num_nodes_visited++] = current_node;

        for (unsigned int y = 0; y < 3; y++) {
            for (unsigned int x = 0; x < 3; x++) {
                printf("%d ", current_node->state[y][x]);
            }
            printf("  ");
            for (unsigned int x = 0; x < 3; x++) {
                printf("%d ", goal_state[y][x]);
            }
            printf("\n");
        }
        printf("\n");

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }
        
        // Extend current node and push children.
        // Both booleans are false as a heuristic is not used in DFS.
        Node** child_nodes = extend_node(current_node, false, false);

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

        counter++;
        if (counter == 100) {
            break;
        }
    }

    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited);
    printf("Num nodes visited: %d\n", num_nodes_visited);
    printf("Num nodes in trace: %d\n", (int)(sizeof(result->trace) / sizeof(result->trace[0])));
}

void breadth_first_search(Node* start_node, SearchResult* result) {
    Queue* nodes_to_visit;
    init_queue(nodes_to_visit);

    Node* nodes_visited[MAX_NODES];
    unsigned int num_nodes_visited = 0;
    
    Node* goal_node;

    // Enqueue start node.
    enqueue(nodes_to_visit, start_node);

    // Visit every node in the queue. When a node is extended in the loop, 
    // the children are enqueued. This causes the nodes to be visited in a BFS order.
    while (!is_empty(nodes_to_visit)) {
        Node* current_node = dequeue(nodes_to_visit);
        nodes_visited[num_nodes_visited++] = current_node;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }

        // Extend current node and enqueue children.
        // Both booleans are false as a heuristic is not used in BFS.
        Node** child_nodes = extend_node(current_node, false, false);

        // Enqueue children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (child_nodes[i] != NULL) {
                enqueue(nodes_to_visit, child_nodes[i]);
            } else {
                break;
            }
        }

        // Free child nodes array to prevent a memory leak.
        free(child_nodes);
    }

    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited);
}

void uniform_cost_search(Node* start_node, SearchResult* result) {

}

void a_star_search(Node* start_node, SearchResult* result) {
    PriorityQueue* nodes_to_visit;
    init_priority_queue(nodes_to_visit);
    
    Node* nodes_visited[MAX_NODES];
    unsigned int num_nodes_visited = 0;

    Node* goal_node;

    // Enqueue start node.
    p_enqueue(nodes_to_visit, start_node);

    // Visit every node in the queue. When a node is extended in the loop,
    // the children are enqueued and sorted in ascending order by their depth + their heuristic.
    // This causes the nodes with the lowest depth + heuristic value to be visited first, hence A* order.
    while (!p_is_empty(nodes_to_visit)) {
        Node* current_node = p_dequeue(nodes_to_visit);
        nodes_visited[num_nodes_visited++] = current_node;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            goal_node = current_node;
            break;
        }

        // Extend current node and enqueue children. 
        // Both booleans are true as both heuristics are used in A*.
        Node** child_nodes = extend_node(current_node, true, true);

        // Enqueue children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (child_nodes[i] != NULL) {
                p_enqueue(nodes_to_visit, child_nodes[i]);
            } else {
                break;
            }
        }

        // Free child nodes array to prevent a memory leak.
        free(child_nodes);
    }

    populate_search_result(result, nodes_visited, goal_node, num_nodes_visited);
}
#include "search_alg.h"

SearchResult* depth_first_search(Node* start_node) {
    Stack* nodes_to_visit;
    init_stack(nodes_to_visit);
    unsigned int num_nodes_visited = 0;

    // Push start node.
    push(nodes_to_visit, start_node);

    // Visit every node in the stack. When a node is extended in the loop,
    // the children are pushed. This causes the nodes to be visited in a DFS order.
    while (!s_is_empty(nodes_to_visit)) {
        Node* current_node = pop(nodes_to_visit);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            // Return the goal node and the number of nodes visited.
            SearchResult result;
            result.goal_node = current_node;
            result.num_nodes_visited = num_nodes_visited;
            return &result;
        }

        // Extend current node and push children. 
        // Both booleans are false as a heuristic is not used in DFS.
        Node* children_nodes[4] = extend_node(current_node, false, false);

        // Push children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (children_nodes[i] != NULL) {
                push(nodes_to_visit, children_nodes[i]);
            } else {
                break;
            }
        }
    }
}

SearchResult* breadth_first_search(Node* start_node) {
    Queue* nodes_to_visit;
    init_queue(nodes_to_visit);
    unsigned int num_nodes_visited = 0;

    // Enqueue start node.
    enqueue(nodes_to_visit, start_node);

    // Visit every node in the queue. When a node is extended in the loop, 
    // the children are enqueued. This causes the nodes to be visited in a BFS order.
    while (!is_empty(nodes_to_visit)) {
        Node* current_node = dequeue(nodes_to_visit);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            // Return the goal node and the number of nodes visited.
            SearchResult result;
            result.goal_node = current_node;
            result.num_nodes_visited = num_nodes_visited;
            return &result;
        }

        // Extend current node and enqueue children. 
        // Both booleans are false as a heuristic is not used in BFS.
        Node* children_nodes[4] = extend_node(current_node, false, false);

        // Enqueue children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (children_nodes[i] != NULL) {
                enqueue(nodes_to_visit, children_nodes[i]);
            } else {
                break;
            }
        }
    }

    // Return result with empty goal node if goal not found.
    SearchResult result;
    result.num_nodes_visited = num_nodes_visited;
}

SearchResult* uniform_cost_search(Node* start_node) {

}

SearchResult* a_star_search(Node* start_node) {
    PriorityQueue* nodes_to_visit;
    init_priority_queue(nodes_to_visit);
    unsigned int num_nodes_visited = 0;

    // Enqueue start node.
    p_enqueue(nodes_to_visit, start_node);

    // Visit every node in the queue. When a node is extended in the loop,
    // the children are enqueued and sorted in ascending order by their depth + their heuristic.
    // This causes the nodes with the lowest depth + heuristic value to be visited first, hence A* order.
    while (!p_is_empty(nodes_to_visit)) {
        Node* current_node = p_dequeue(nodes_to_visit);
        num_nodes_visited++;

        // Check if current node is the goal state.
        if (check_states_are_equivalent(current_node->state, goal_state)) {
            // Return the goal node and the number of nodes visited.
            SearchResult result;
            result.goal_node = current_node;
            result.num_nodes_visited = num_nodes_visited;
            return &result;
        }

        // Extend current node and enqueue children. 
        // Both booleans are true as both heuristics are used in A*.
        Node* children_nodes[4] = extend_node(current_node, true, true);

        // Enqueue children nodes.
        for (unsigned int i = 0; i < 4; i++) {
            if (children_nodes[i] != NULL) {
                p_enqueue(nodes_to_visit, children_nodes[i]);
            } else {
                break;
            }
        }
    }

    // Return result with empty goal node if goal not found.
    SearchResult result;
    result.num_nodes_visited = num_nodes_visited;
}
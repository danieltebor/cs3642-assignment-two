#include "eight_puzzle.h"

// Goal state.
const unsigned int goal_state[3][3] = {
    {1, 2, 3},
    {8, 0, 4},
    {7, 6, 5}
};

// Generate a node with a random starting state.
Node* generate_random_start_node() {
    Node* node = (Node*) malloc(sizeof(Node));
    
    // Copy goal state into node.
    unsigned int blank_x, blank_y;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            node->state[y][x] = goal_state[y][x];
            if (node->state[y][x] == 0) {
                blank_x = x;
                blank_y = y;
            }
        }
    }
    srand(time(NULL));

    // Randomly scramble the state. This garuntees the state can be solved.
    for (unsigned int i = 0; i < 10000; i++) {
        // Generate a random move.
        unsigned int move = rand() % 4;

        if (move == 0 && blank_x > 0) {
            // Swap blank tile with tile to the left of it.
            unsigned int left_tile = node->state[blank_y][blank_x - 1];
            node->state[blank_y][blank_x - 1] = 0;
            node->state[blank_y][blank_x] = left_tile;
            blank_x--;
        }
        else if (move == 1 && blank_y < 2) {
            // Swap blank tile with tile below it.
            unsigned int below_tile = node->state[blank_y + 1][blank_x];
            node->state[blank_y + 1][blank_x] = 0;
            node->state[blank_y][blank_x] = below_tile;
            blank_y++;
        }
        else if (move == 2 && blank_x < 2) {
            // Swap blank tile with tile to the right of it.
            unsigned int right_tile = node->state[blank_y][blank_x + 1];
            node->state[blank_y][blank_x + 1] = 0;
            node->state[blank_y][blank_x] = right_tile;
            blank_x++;
        }
        else if (move == 3 && blank_y > 0) {
            // Swap blank tile with tile above it.
            unsigned int above_tile = node->state[blank_y - 1][blank_x];
            node->state[blank_y - 1][blank_x] = 0;
            node->state[blank_y][blank_x] = above_tile;
            blank_y--;
        }
        else {
            i--;
        }
    }

    // Populate rest of node.
    node->parent = NULL;
    node->depth = 0;
    node->heuristic = UINT_MAX;

    return node;
}

// Calculates a heuristic value corresponding to the number 
// of tiles misplaced compared to the goal state
unsigned int calc_num_tiles_misplaced(Node* node) {
    unsigned int num_tiles_misplaced = 0;

    // Traverse state.
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            // Increment num_tiles_misplaced if tile is misplaced.
            if (node->state[y][x] != goal_state[y][x]) {
                num_tiles_misplaced++;
            }
        }
    }

    return num_tiles_misplaced;
}

// Calculates a heuristic value by finding the 
// Manhatten distance of a tile from its position in the goal state.
unsigned int calc_manhatten_distance(unsigned int tile_value, unsigned int x, unsigned int y) {
    unsigned int goal_x, goal_y;

    // Find goal position of tile.
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            if (goal_state[y][x] == tile_value) {
                goal_x = x;
                goal_y = y;
            }
        }
    }

    // Calculate manhatten distance.
    return abs(goal_y - y) + abs(goal_x - x);
}

// Calculates a heuristic value by calculating the sum of
// the Manhatten distances of all tiles in the state compared to the goal.
unsigned int calc_summed_manhatten_distances(Node* node) {
    unsigned int total_distances = 0;

    // Traverse state.
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            unsigned int tile_value = node->state[y][x];
            if (tile_value != 0) {
                total_distances += calc_manhatten_distance(tile_value, x, y);
            }
        }
    }

    return total_distances;
}

// Checks if two nodes are equivalent.
bool check_states_are_equivalent(const unsigned int state1[3][3], const unsigned int state2[3][3]) {
    // Traverse state.
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            // Check if tile is misplaced. If so return false.
            if (state1[y][x] != state2[y][x]) {
                return false;
            }
        }
    }
    return true;
}

// Trace back to root checking for any duplicate states.
bool state_is_repeat(unsigned int state[3][3],
                     const Node** nodes_visited,
                     unsigned int num_nodes_visited) {
    // Iterate through nodes_visited starting from the end.
    // Starting from the end is ideal because one of the child nodes
    // Will always have the same state as the parent node of the node being extended.
    for (int i = num_nodes_visited - 1; i >= 0; i--) {
        if (check_states_are_equivalent(state, nodes_visited[i]->state)) {
            return true;
        }
    }

    return false;
}

// Return all possible children of a node.
Node** extend_node(const Node* node,
                   const Node** nodes_visited,
                   unsigned int num_nodes_visited,
                   bool should_use_misplaced_heuristic,
                   bool should_use_manhatten_distance_heuristic) {
    // Find blank tile.
    unsigned int blank_x = 0, blank_y = 0;
    bool blank_tile_found = false;

    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            if (node->state[y][x] == 0) {
                blank_x = x;
                blank_y = y;
                blank_tile_found = true;
                break;
            }
        }

        if (blank_tile_found) {
            break;
        }
    }

    // Generate child nodes starting from up and going clockwise.
    // Dynamically allocate so that array persists outside of function scope.
    Node** child_nodes = calloc(4, sizeof(Node*));
    unsigned int child_node_idx = 0;

    for (unsigned int i = 0; i < 4; i++) {
        // Dynamically allocate so that node persists outside of function scope.
        Node* child_node = malloc(sizeof(Node));
        // Copy state into child_node's state.
        memcpy(child_node->state, node->state, sizeof(node->state));
        
        if (i == 0 && blank_x > 0) {
            // Swap blank tile with tile to the left of it.
            unsigned int left_tile = child_node->state[blank_y][blank_x - 1];
            child_node->state[blank_y][blank_x - 1] = 0;
            child_node->state[blank_y][blank_x] = left_tile;
        }
        else if (i == 1 && blank_y < 2) {
            // Swap blank tile with tile below it.
            unsigned int below_tile = child_node->state[blank_y + 1][blank_x];
            child_node->state[blank_y + 1][blank_x] = 0;
            child_node->state[blank_y][blank_x] = below_tile;
        }
        else if (i == 2 && blank_x < 2) {
            // Swap blank tile with tile to the right of it.
            unsigned int right_tile = child_node->state[blank_y][blank_x + 1];
            child_node->state[blank_y][blank_x + 1] = 0;
            child_node->state[blank_y][blank_x] = right_tile;
        }
        else if (i == 3 && blank_y > 0) {
            // Swap blank tile with tile above it.
            unsigned int above_tile = child_node->state[blank_y - 1][blank_x];
            child_node->state[blank_y - 1][blank_x] = 0;
            child_node->state[blank_y][blank_x] = above_tile;
        }
        else {
            // Free child_node if it is not a valid child.
            free(child_node);
            continue;
        }

        // Populate blank fields in child_node.
        child_node->parent = node;
        child_node->depth = node->depth + 1;
        child_node->heuristic = 0;
        if (should_use_misplaced_heuristic) {
            child_node->heuristic += calc_num_tiles_misplaced(child_node);
        }
        if (should_use_manhatten_distance_heuristic) {
            child_node->heuristic += calc_summed_manhatten_distances(child_node);
        }

        // Add child_node to child_nodes if it's state does not already exist.
        if (!state_is_repeat(child_node->state, nodes_visited, num_nodes_visited)) {
            child_nodes[child_node_idx++] = child_node;
        } else {
            // Free child_node if it is a repeat.
            free(child_node);
        }
    }

    return child_nodes;
}
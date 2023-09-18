#include "eight_puzzle.h"

// Goal state.
unsigned int goal_state[3][3] = {
    {1, 2, 3},
    {8, 0, 4},
    {7, 6, 5}
};

// Generate a node with a random starting state.
Node generate_random_start_node() {
    Node node;
    
    // Generate a random starting state.
    unsigned int tiles[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    bool used[9] = {false};
    srand(time(NULL));

    // Traverse state.
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            // Randomly select a tile from tiles for the current
            // State coordinate. Ensure that the tile is not already
            // in use.
            bool tile_placement_is_valid = false;

            while (!tile_placement_is_valid) {
                unsigned const int RANDOM_INDEX = rand() % 9;

                if (!used[RANDOM_INDEX]) {
                    node.state[y][x] = tiles[RANDOM_INDEX];
                    used[RANDOM_INDEX] = true;
                    tile_placement_is_valid = true;
                }
            }
        }
    }

    // Populate rest of node.
    node.parent = NULL;
    node.depth = 0;
    node.heuristic = UINT_MAX;

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
bool check_states_are_equivalent(unsigned int state1[3][3], unsigned int state2[3][3]) {
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
bool state_is_repeat(Node* node, unsigned int state[3][3]) {
    // Trace back to root.
    Node* current_node = node;
    while (current_node != NULL) {
        // Check if state is equivalent to node's state.
        if (check_states_are_equivalent(current_node->state, state)) {
            return true;
        }

        // Move to parent node.
        current_node = current_node->parent;
    }

    return false;
}

// Return all possible children of a node.
Node** extend_node(Node* node,
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
        
        if (i == 0 && blank_y > 0) {
            // Swap blank tile with tile above it.
            unsigned int above_tile = child_node->state[blank_y - 1][blank_x];
            child_node->state[blank_y - 1][blank_x] = 0;
            child_node->state[blank_y][blank_x] = above_tile;
        }
        else if (i == 1 && blank_x < 2) {
            // Swap blank tile with tile to the right of it.
            unsigned int right_tile = child_node->state[blank_y][blank_x + 1];
            child_node->state[blank_y][blank_x + 1] = 0;
            child_node->state[blank_y][blank_x] = right_tile;
        }
        else if (i == 2 && blank_y < 2) {
            // Swap blank tile with tile below it.
            unsigned int below_tile = child_node->state[blank_y + 1][blank_x];
            child_node->state[blank_y + 1][blank_x] = 0;
            child_node->state[blank_y][blank_x] = below_tile;
        }
        else if (i == 3 && blank_x > 0) {
            // Swap blank tile with tile to the left of it.
            unsigned int left_tile = child_node->state[blank_y][blank_x - 1];
            child_node->state[blank_y][blank_x - 1] = 0;
            child_node->state[blank_y][blank_x] = left_tile;
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
        if (!state_is_repeat(node, child_node->state)) {
            child_nodes[child_node_idx++] = child_node;
        } else {
            // Free child_node if it is a repeat.
            free(child_node);
        }
    }

    return child_nodes;
}
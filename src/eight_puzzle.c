#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "eight_puzzle.h"
#include "hash_table.h"

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
        // Generate a random move 0-3.
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
unsigned int calc_num_tiles_misplaced(const Node* node) {
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

const unsigned int X_CLOCKWISE_ORDER[8] = {0, 1, 2, 2, 2, 1, 0, 0};
const unsigned int Y_CLOCKWISE_ORDER[8] = {0, 0, 0, 1, 2, 2, 2, 0};

// Add Nilsson's sequence score heuristic
unsigned int calc_nilsson_sequence_score(const Node* node) {
    unsigned int score = 0;

    // Add one to score if center not 0.
    if (node->state[1][1] != 0) {
        score++;
    }

    // Build state clockwise.
    unsigned int goal_state_clockwise[8];
    unsigned int node_state_clockwise[8];

    for (unsigned int i = 0; i < 8; i++) {
        goal_state_clockwise[i] = goal_state[Y_CLOCKWISE_ORDER[i]][X_CLOCKWISE_ORDER[i]];
        node_state_clockwise[i] = node->state[Y_CLOCKWISE_ORDER[i]][X_CLOCKWISE_ORDER[i]];
    }

    // Check if each tile clockwise matches 
    for (unsigned int i = 0; i < 7; i++) {
        if (node_state_clockwise[i] != goal_state_clockwise[i]) {
            score += 2;
        }
    }

    return score * 3;
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
unsigned int calc_summed_manhatten_distances(const Node* node) {
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

// Search nodes_visited for any duplicate states.
bool state_is_repeat(const Node* node,
                     const Node** nodes_visited) {
    // Check if Node already exists in nodes_visited.
    if (search(node, nodes_visited, MAX_NODES) == NULL) {
        return false;
    }

    return true;
}

// Return all possible children of a node.
Node** extend_node(const Node* node,
                   const Node** nodes_visited,
                   bool should_use_heuristic) {
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
        if (should_use_heuristic) {
            child_node->heuristic += calc_nilsson_sequence_score(child_node) + calc_summed_manhatten_distances(child_node);
        }

        // Add child_node to child_nodes if it's state does not already exist.
        if (!state_is_repeat(child_node, nodes_visited)) {
            child_nodes[child_node_idx++] = child_node;
        } else {
            // Free child_node if it is a repeat.
            free(child_node);
        }
    }

    return child_nodes;
}
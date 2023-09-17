#include "eight_puzzle.h"

// Generate a node with a random starting state.
Node generate_random_start_node() {
    Node node;
    
    // Generate a random starting state.
    unsigned int tiles[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
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

                if (tiles[RANDOM_INDEX] != NULL) {
                    node.state[y][x] = tiles[RANDOM_INDEX];
                    tiles[RANDOM_INDEX] = NULL;
                    tile_placement_is_valid = true;
                }
            }
        }
    }

    // Populate rest of node.
    node.parent = NULL;
    node.cost = 0;

    return node;
}

// Checks if two nodes are equivalent.
bool check_states_are_equivalent(unsigned int* state1[3][3], unsigned int* state2[3][3]) {
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
                total_distances += CalcManhattenDistance(tile_value, x, y);
            }
        }
    }
};